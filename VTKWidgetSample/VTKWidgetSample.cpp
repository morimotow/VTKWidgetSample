// VTKWidgetSample.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"

// VTK機能の利用に必要
#include "vtkAutoInit.h" 
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

#include <vtkSmartPointer.h>
#include <vtkVRMLImporter.h>

#include <vtkActorCollection.h>
#include <vtkActor.h>

#include <vtkRendererCollection.h>
#include <vtkRenderer.h>

#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTerrain.h>

#include <vtkImageReader2Factory.h>
#include <vtkImageReader2.h>
#include <vtkTexture.h>

#include <vtkNamedColors.h>

#include <vtkHandleWidget.h>
#include <vtkAngleWidget.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>
#include <vtkDistanceRepresentation3D.h>

#include <vtkWidgetRepresentation.h>
#include <vtkHandleRepresentation.h>
#include <vtkPointHandleRepresentation2D.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkProperty.h>
#include <vtkProperty2D.h>


#include <vtkSphereHandleRepresentation.h>

#include <vtkSeedWidget.h>
#include <vtkSeedRepresentation.h>

#include <vtkMapper.h>
#include <vtkPolyDataCollection.h>
#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkCellCentersPointPlacer.h>

#include <vtkFixedSizeHandleRepresentation3D.h>

#include <vtkLineWidget2.h>
#include <vtkLineRepresentation.h>

#include <vtkLineSource.h>
#include <vtkPolyDataMapper.h>

#include <vtkPoints.h>
#include <vtkLine.h>
#include <vtkTriangle.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>


void LoadFile();
void LoadTexture();
vtkSmartPointer<vtkSeedWidget> AddHandle(float x, float y, float z);

vtkSmartPointer<vtkRenderer> renderer;
vtkSmartPointer<vtkRenderWindow> renderWindow;
vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
vtkSmartPointer<vtkHandleWidget> CreateHandle(double* position, vtkPolygonalSurfacePointPlacer* placer);
void SetupPolyDataMapper(double* p1, double* p2, double* p3);

vtkSmartPointer<vtkHandleWidget> handleWidget1;
vtkSmartPointer<vtkHandleWidget> handleWidget2;
vtkSmartPointer<vtkHandleWidget> handleWidget3;

vtkSmartPointer<vtkLineSource> line1;
vtkSmartPointer<vtkLineSource> line2;
vtkSmartPointer<vtkLineSource> line3;

vtkSmartPointer<vtkPolyData> polyData2;
vtkSmartPointer<vtkPolyDataMapper> dataMapper;
vtkSmartPointer<vtkRenderer> renderer2;

class vtkHandleCallback : public vtkCommand
{
public:
	static vtkHandleCallback *New()
	{
		return new vtkHandleCallback;
	}

	virtual void Execute(vtkObject *caller, unsigned long, void*)
	{

		vtkHandleWidget *handleWidget =
			reinterpret_cast<vtkHandleWidget*>(caller);

		vtkHandleRepresentation * rep = static_cast<vtkHandleRepresentation*>(handleWidget->GetRepresentation());
		double pos[3];
		rep->GetWorldPosition(pos);


		pos[2] = 0.0;
		vtkPoints * points = polyData2->GetPoints();
		if (handleWidget == handleWidget1)
		{
			points->SetPoint(0, pos);
			points->Modified();
		}
		if (handleWidget == handleWidget2) {
			points->SetPoint(1, pos);
			points->Modified();
		}
		if (handleWidget == handleWidget3) {
			points->SetPoint(2, pos);
			points->Modified();
		}

//		cout << "P: " << pos[0] << " " << pos[1] << " " << pos[2] << endl;
	}
	vtkHandleCallback() {}

};


int main(int argc, char *argv[])
{

	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

	renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetLayer(0);
	renderWindow = vtkSmartPointer<vtkRenderWindow>::New();

	// 2段目のレイヤーを作成(カメラは1段目の奴を使用)
	renderer2 = vtkSmartPointer<vtkRenderer>::New();
	renderer2->SetActiveCamera(renderer->GetActiveCamera());
	renderer2->SetLayer(1);

	renderWindow->AddRenderer(renderer);
	renderWindow->AddRenderer(renderer2);
	renderWindow->SetNumberOfLayers(2);

	renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	// オブジェクト読み込み
	LoadFile();
	LoadTexture();

	// マウス操作スタイルを設定する
	vtkSmartPointer<vtkInteractorStyleTerrain> style = vtkSmartPointer<vtkInteractorStyleTerrain>::New();
	renderWindowInteractor->SetInteractorStyle(style);

	// ここでハンドル追加？

// ===========================================================

	// ハンドルを配置するポリゴンデータを含むActorを取得
	vtkSmartPointer<vtkActorCollection> actors = vtkSmartPointer<vtkActorCollection>::New();
	actors = renderer->GetActors();
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor = actors->GetLastActor();

	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData = vtkPolyData::SafeDownCast(actor->GetMapper()->GetInputAsDataSet());

	vtkSmartPointer<vtkPolygonalSurfacePointPlacer> placer = vtkSmartPointer<vtkPolygonalSurfacePointPlacer>::New();
	placer->AddProp(actor);
	placer->GetPolys()->AddItem(polyData);



	double p1[3] = { -28.598, 201.880, -0.202 };
	double p2[3] = { 35.651, 175.682, -0.610 };
	double p3[3] = { 2.921, 36.699, -0.392 };

	SetupPolyDataMapper(p1, p2, p3);

	// 母指球
	handleWidget1 = CreateHandle(p1, placer);

	// 小指球
	handleWidget2 = CreateHandle(p2, placer);

	// かかと中央
	handleWidget3 = CreateHandle(p3, placer);




	// 手動で点を追加(http://vtk.1045678.n5.nabble.com/Programmatically-adding-and-removing-seeds-from-vtkSeedWidget-td1245744.html)
//https://www.vtk.org/Wiki/VTK/Examples/Cxx/Widgets/SeedWidgetImage
	  //	seedWidget->CompleteInteraction();
//	seedRep->BuildRepresentation();

//	double p1[3] = { -28.598, 201.880, -0.202 };
////	double p1[2] = { 0.5, 0.5};
//	int s1 = seedRep->CreateHandle(p1);
//	seedRep->SetSeedDisplayPosition(s1, p1);
//
//	double p2[3] = { 35.651, 175.682, -0.610 };
////	int s2 = seedRep->CreateHandle(p2);
////	seedRep->SetSeedDisplayPosition(s2, p2);
//
//	seedRep->BuildRepresentation();
	//	seedWidget->RestartInteraction();


	// ===========================================================

//	vtkSeedWidget* firstMetHandle = AddHandle(-28.598, 201.880, -0.202);
//	vtkHandleWidget* fifthMetHandle = AddHandle(35.651, 175.682, -0.610);

	renderer->SetBackground(colors->GetColor3d("DimGray").GetData());
	renderWindow->SetSize(480, 800);

	// スムージングしてみる
	renderWindow->PolygonSmoothingOn();

	renderWindow->Render();
	renderWindowInteractor->Initialize();
	renderWindow->Render();

	handleWidget1->EnabledOn();
	handleWidget2->EnabledOn();
	handleWidget3->EnabledOn();

	renderWindow->Render();

	renderer->ResetCamera();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}

// ハンドルを作成
vtkSmartPointer<vtkHandleWidget> CreateHandle(double* position, vtkPolygonalSurfacePointPlacer* placer)
{

	vtkSmartPointer<vtkFixedSizeHandleRepresentation3D> point = vtkSmartPointer<vtkFixedSizeHandleRepresentation3D>::New();

	vtkSmartPointer<vtkHandleWidget> handleWidget = vtkSmartPointer<vtkHandleWidget>::New();
	handleWidget->SetInteractor(renderWindowInteractor);
	handleWidget->SetRepresentation(point);
	point->SetPointPlacer(placer);

	handleWidget->AllowHandleResizeOff();

	point->SetWorldPosition(position);
	point->ConstrainedOn();
	point->SetHandleSize(10.0f);
	point->SmoothMotionOff();
	point->GetProperty()->SetColor(0, 1, 0);
	handleWidget->EnableAxisConstraintOff();

	// イベントに応答できるようにする
	vtkSmartPointer<vtkHandleCallback> boxCallback =
		vtkSmartPointer<vtkHandleCallback>::New();
	handleWidget->AddObserver(vtkCommand::InteractionEvent, boxCallback);

	return handleWidget;
}

void SetupPolyDataMapper(double* p1, double* p2, double* p3) {
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points->InsertNextPoint(p1[0], p1[1], 0.0);
	points->InsertNextPoint(p2[0], p2[1], 0.0);
	points->InsertNextPoint(p3[0], p3[1], 0.0);

	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
	vtkIdType ids1[2] = { 0, 1}; // 第一-第五中足骨
	vtkIdType ids2[3] = { 1, 2, 0 }; // 踵骨 - 第一～第五
	cells->InsertNextCell(2, ids1);
	cells->InsertNextCell(3, ids2);

	polyData2 = vtkSmartPointer<vtkPolyData>::New();
	polyData2->SetPoints(points);
	polyData2->SetLines(cells);

	dataMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	dataMapper->SetInputData(polyData2);

	vtkSmartPointer<vtkActor> polyActor = vtkSmartPointer<vtkActor>::New();
	polyActor->SetMapper(dataMapper);

	renderer2->AddActor(polyActor);
}

void LoadFile()
{
	// VRML Import
	vtkSmartPointer<vtkVRMLImporter> importer = vtkSmartPointer<vtkVRMLImporter>::New();
	importer->SetFileName("L.wrl");
	importer->SetRenderWindow(renderWindow);
	importer->Update();

	vtkSmartPointer<vtkActorCollection> actors = vtkSmartPointer<vtkActorCollection>::New();
	actors = renderer->GetActors();
	std::cout << "There are " << actors->GetNumberOfItems() << " actors" << std::endl;

}

void LoadTexture()
{

	// テクスチャ用イメージの読み込み
	vtkSmartPointer<vtkImageReader2Factory> readerFactory = vtkSmartPointer<vtkImageReader2Factory>::New();
	vtkImageReader2 *textureFile = readerFactory->CreateImageReader2("L.jpg");
	textureFile->FileLowerLeftOn();  // 左右反転の指示？
	textureFile->SetFileName("L.jpg");
	textureFile->Update();

	// テクスチャデータを作成
	vtkSmartPointer<vtkTexture> texture = vtkSmartPointer<vtkTexture>::New();
	texture->SetInputConnection(textureFile->GetOutputPort());
	texture->InterpolateOn();
	texture->RepeatOff();
	texture->Update();

	// 表示中のVRMLデータ(Actor)を取得
	vtkSmartPointer<vtkActorCollection> actors = vtkSmartPointer<vtkActorCollection>::New();
	actors = renderer->GetActors();
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor = actors->GetLastActor();

	// Actorにテクスチャデータを設定
	actor->SetTexture(texture);

}

vtkSmartPointer<vtkSeedWidget> AddHandle(float x, float y, float z)
{


	vtkSmartPointer<vtkPointHandleRepresentation2D> point = vtkSmartPointer<vtkPointHandleRepresentation2D>::New();
	double pos[3] = { (double)x, (double)y, (double)z };
//	point->SetHandleSize(10);
//	point->SetWorldPosition(pos);
	point->GetProperty()->SetColor(1, 0, 0);
//	point->VisibilityOn();
//	point->SetRenderer(renderer);

	vtkSmartPointer<vtkSeedRepresentation> seedRep = vtkSmartPointer<vtkSeedRepresentation>::New();
	seedRep->SetHandleRepresentation(point);

	// Create the seed widget
	vtkSmartPointer<vtkSeedWidget> seedWidget = vtkSmartPointer<vtkSeedWidget>::New();
	seedWidget->SetInteractor(renderWindowInteractor);
	seedWidget->SetRepresentation(seedRep);

	seedWidget->On();
	return seedWidget;
//
//	vtkSmartPointer<vtkHandleWidget> widget1 = vtkSmartPointer<vtkHandleWidget>::New();
//	widget1->SetRepresentation(point);
////	widget1->CreateDefaultRepresentation();
//	widget1->EnabledOn();
//	widget1->On();
//	widget1->SetDefaultRenderer(renderer);
//	widget1->SetInteractor(renderWindowInteractor);
//	widget1->Render();
//
//
//
//	
//	return widget1;
}




// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
