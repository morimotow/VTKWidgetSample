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

#include <vtkWidgetRepresentation.h>
#include <vtkHandleRepresentation.h>
#include <vtkPointHandleRepresentation2D.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkProperty.h>
#include <vtkProperty2D.h>

#include <vtkSeedWidget.h>
#include <vtkSeedRepresentation.h>

void LoadFile();
void LoadTexture();
vtkSmartPointer<vtkSeedWidget> AddHandle(float x, float y, float z);

vtkSmartPointer<vtkRenderer> renderer;
vtkSmartPointer<vtkRenderWindow> renderWindow;
vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;

int main(int argc, char *argv[])
{

	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

	renderer = vtkSmartPointer<vtkRenderer>::New();
	renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);

	renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	// オブジェクト読み込み
	LoadFile();
	LoadTexture();

	// マウス操作スタイルを設定する
	vtkSmartPointer<vtkInteractorStyleTerrain> style = vtkSmartPointer<vtkInteractorStyleTerrain>::New();
//	renderWindowInteractor->SetInteractorStyle(style);

	// ここでハンドル追加？

// ===========================================================

	// 母指球
	vtkSmartPointer<vtkPointHandleRepresentation3D> point1 = vtkSmartPointer<vtkPointHandleRepresentation3D>::New();
	double p1[3] = { -28.598, 201.880, -0.202 };
	point1->SetWorldPosition(p1);

	vtkSmartPointer<vtkHandleWidget> handleWidget1 = vtkSmartPointer<vtkHandleWidget>::New();
	handleWidget1->SetRepresentation(point1);
	handleWidget1->SetInteractor(renderWindowInteractor);

	// 小指球
	vtkSmartPointer<vtkPointHandleRepresentation3D> point2 = vtkSmartPointer<vtkPointHandleRepresentation3D>::New();
	double p2[3] = { 35.651, 175.682, -0.610 };
	point2->SetWorldPosition(p2);

	vtkSmartPointer<vtkHandleWidget> handleWidget2 = vtkSmartPointer<vtkHandleWidget>::New();
	handleWidget2->SetRepresentation(point2);
	handleWidget2->SetInteractor(renderWindowInteractor);

	// かかと中央
	vtkSmartPointer<vtkPointHandleRepresentation3D> point3 = vtkSmartPointer<vtkPointHandleRepresentation3D>::New();
	double p3[3] = { 2.921, 36.699, -0.392 };
	point3->SetWorldPosition(p3);

	vtkSmartPointer<vtkHandleWidget> handleWidget3 = vtkSmartPointer<vtkHandleWidget>::New();
	handleWidget3->SetRepresentation(point3);
	handleWidget3->SetInteractor(renderWindowInteractor);


	


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
