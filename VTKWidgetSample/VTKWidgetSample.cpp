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

void LoadFile(vtkRenderWindow* renderWindow);
void LoadTexture(vtkRenderWindow* renderWindow);

int main(int argc, char *argv[])
{

	vtkSmartPointer<vtkNamedColors> colors = vtkSmartPointer<vtkNamedColors>::New();

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	// オブジェクト読み込み
	LoadFile(renderWindow);
	LoadTexture(renderWindow);

	// マウス操作スタイルを設定する
	vtkSmartPointer<vtkInteractorStyleTerrain> style = vtkSmartPointer<vtkInteractorStyleTerrain>::New();
	renderWindowInteractor->SetInteractorStyle(style);

	renderer->SetBackground(colors->GetColor3d("DimGray").GetData());
	renderWindow->SetSize(480, 800);
	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}

void LoadFile(vtkRenderWindow* renderWindow)
{
	// VRML Import
	vtkSmartPointer<vtkVRMLImporter> importer = vtkSmartPointer<vtkVRMLImporter>::New();
	importer->SetFileName("L.wrl");
	importer->SetRenderWindow(renderWindow);
	importer->Update();

	vtkSmartPointer<vtkRendererCollection> renderers = vtkSmartPointer<vtkRendererCollection>::New();
	renderers = renderWindow->GetRenderers();
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer = renderers->GetFirstRenderer();

	vtkSmartPointer<vtkActorCollection> actors = vtkSmartPointer<vtkActorCollection>::New();
	actors = renderer->GetActors();
	std::cout << "There are " << actors->GetNumberOfItems() << " actors" << std::endl;

}

void LoadTexture(vtkRenderWindow* renderWindow)
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
	vtkSmartPointer<vtkRendererCollection> renderers = vtkSmartPointer<vtkRendererCollection>::New();
	renderers = renderWindow->GetRenderers();
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer = renderers->GetFirstRenderer();

	vtkSmartPointer<vtkActorCollection> actors = vtkSmartPointer<vtkActorCollection>::New();
	actors = renderer->GetActors();
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor = actors->GetLastActor();

	// Actorにテクスチャデータを設定
	actor->SetTexture(texture);

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
