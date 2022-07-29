#include "Vector3.h"
#include "Matrix4.h"
#include <DxLib.h>
#include <cstring>  // memcpy

// 球(Sphere)の描画
// DxLib => int DrawSphere3D(VECTOR CenterPos, float r, int DivNum, unsigned int DifColor, unsigned int SpcColor, int FillFlag);
int DrawSphere3D(const Vector3& CenterPos, const float r, const int DivNum, const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag);

// 円錐(Cone)の描画
// DxLib => int DrawCone3D( VECTOR TopPos, VECTOR BottomPos, float r, int DivNum, unsigned int DifColor, unsigned int SpcColor, int FillFlag);
int DrawCone3D(const Vector3& TopPos, const Vector3& BottomPos, const float r,
    const int DivNum, const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag);

// 線分の描画
// DxLib => int DrawLine3D(VECTOR Pos1, VECTOR Pos2, unsigned int Color);
int DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, const unsigned int Color);

// カメラの位置と姿勢の設定
// DxLib => int SetCameraPositionAndTargetAndUpVec(VECTOR Position, VECTOR Target, VECTOR Up);
int SetCameraPositionAndTargetAndUpVec(
    const Vector3& cameraPosition,    // カメラの位置
    const Vector3& cameraTarget,      // カメラの注視点
    const Vector3& cameraUp		    // カメラの上の向き
);

// モデルの座標変換用行列をセットする
// DxLib => int MV1SetMatrix(int MHandle, MATRIX Matrix);
int MV1SetMatrix(const int MHandle, const Matrix4 Matrix);


// 関数プロトタイプ宣言
void DrawAxis3D(const float length);	// x,y,z 軸の描画
void DrawKeyOperation();                // キー操作の描画

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // DxLib 初期化
    const int WindowWidth = 1600;
    const int WindowHeight = 900;

    ChangeWindowMode(TRUE);	    					// ウインドウモードにする
    SetGraphMode(WindowWidth, WindowHeight, 32);	// 画面モードのセット
    SetBackgroundColor(0, 0, 64);       // 背景色 RGB
    if (DxLib_Init() == -1) return -1;	// DxLib 初期化処理
    SetDrawScreen(DX_SCREEN_BACK);		// 描画先画面を裏画面にセット

    SetUseZBuffer3D(TRUE);      // Ｚバッファを有効にする
    SetWriteZBuffer3D(TRUE);    // Ｚバッファへの書き込みを有効にする

    // カメラ初期化
    Vector3 cameraPosition(50.0f, 50.0f, -400.0f);
    Vector3 cameraTarget(0.0f, 0.0f, 0.0f);
    Vector3 cameraUp(0.0f, 1.0f, 0.0f);

    //    クリップ面  近     遠
    SetCameraNearFar(1.0f, 10000.0f);		//カメラの有効範囲を設定
    SetCameraScreenCenter(WindowWidth / 2.0f, WindowHeight / 2.0f);  // 画面の中心をカメラの中心に合わせる
    SetCameraPositionAndTargetAndUpVec(cameraPosition, cameraTarget, cameraUp);

    // アプリで使用する変数
    int model = MV1LoadModel("fighter.mqo");

    // xyz軸の回転角度
    const float ROT_UNIT = 0.01f;
    float rotX = 0.0f;
    float rotY = 0.0f;
    float rotZ = 0.0f;

    // ゲームループ
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        // 更新 ---------------------------
        if (CheckHitKey(KEY_INPUT_A))   rotY += ROT_UNIT;
        if (CheckHitKey(KEY_INPUT_D))   rotY -= ROT_UNIT;

        if (CheckHitKey(KEY_INPUT_W))   rotX += ROT_UNIT;
        if (CheckHitKey(KEY_INPUT_S))   rotX -= ROT_UNIT;

        if (CheckHitKey(KEY_INPUT_E))   rotZ += ROT_UNIT;
        if (CheckHitKey(KEY_INPUT_Z))   rotZ -= ROT_UNIT;

        // [R] でリセット
        if (CheckHitKey(KEY_INPUT_R))
        {
            rotX = rotY = rotZ = 0;
        }

        // 各種変換行列の計算
        Matrix4 matScale = scale(Vector3(5.1f, 5.1f, 5.1f));	// モデルの拡大率

        Matrix4 matRotX = rotateX(rotX);
        Matrix4 matRotY = rotateY(rotY);
        Matrix4 matRotZ = rotateZ(rotZ);

        Matrix4 matRot = matRotX * matRotY * matRotZ;

        Matrix4 matWorld = matScale * matRot;

        MV1SetMatrix(model, matWorld);

        // 描画 ---------------------------
        ClearDrawScreen();		// 画面を消去
        DrawAxis3D(200.0f);     // xyz軸の描画

        // モデルの描画
        MV1DrawModel(model);

        // キー操作の描画
        DrawKeyOperation();

        // フリップする
        ScreenFlip();
    }
    DxLib::DxLib_End(); 	// DxLib 終了処理
    return 0; 	// ソフトの終了
}

// xyz軸の描画
void DrawAxis3D(const float length)
{
    // 軸の線の描画
    DrawLine3D(Vector3(-length, 0, 0), Vector3(+length, 0, 0), GetColor(255, 0, 0));	// x軸
    DrawLine3D(Vector3(0, -length, 0), Vector3(0, +length, 0), GetColor(0, 255, 0));	// y軸
    DrawLine3D(Vector3(0, 0, -length), Vector3(0, 0, +length), GetColor(0, 0, 255));	// z軸

    // 軸の先の描画
    const float coneSize = 10.0f;
    DrawCone3D(Vector3(length, 0, 0), Vector3(length - coneSize, 0, 0), coneSize / 2, 16, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);
    DrawCone3D(Vector3(0, length, 0), Vector3(0, length - coneSize, 0), coneSize / 2, 16, GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);
    DrawCone3D(Vector3(0, 0, length), Vector3(0, 0, length - coneSize), coneSize / 2, 16, GetColor(0, 0, 255), GetColor(255, 255, 255), TRUE);
}

// キー操作の描画
void DrawKeyOperation()
{
    const unsigned white = GetColor(255, 255, 255);

    DrawFormatString(10, 20 * 1, white, "  [W][E][R]  R : リセット");
    DrawFormatString(10, 20 * 2, white, "[A][S][D]    AD: y軸まわりの回転");
    DrawFormatString(10, 20 * 3, white, " [Z]         WS: x軸まわりの回転");
    DrawFormatString(10, 20 * 4, white, "             EZ: z軸まわりの回転");
}

// Vector3型を利用しやすくするための オーバーロード関数群

// 球の描画
// DxLib => int DrawSphere3D(VECTOR CenterPos, float r, int DivNum, unsigned int DifColor, unsigned int SpcColor, int FillFlag);
int DrawSphere3D(const Vector3& CenterPos, const float r, const int DivNum, const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag)
{
    VECTOR centerPos = { CenterPos.x, CenterPos.y, CenterPos.z };	// 構造体 初期化子リスト

    return DrawSphere3D(centerPos, r, DivNum, DifColor, SpcColor, FillFlag);
}

// DxLib => int DrawCone3D( VECTOR TopPos, VECTOR BottomPos, float r, int DivNum, unsigned int DifColor, unsigned int SpcColor, int FillFlag);
int DrawCone3D(const Vector3& TopPos, const Vector3& BottomPos, const float r,
    const int DivNum, const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag)
{
    VECTOR topPos = { TopPos.x, TopPos.y, TopPos.z };
    VECTOR bottomPos = { BottomPos.x, BottomPos.y, BottomPos.z };

    return DrawCone3D(topPos, bottomPos, r, DivNum, DifColor, SpcColor, FillFlag);
}

// 線分の描画
// DxLib => int DrawLine3D(VECTOR Pos1, VECTOR Pos2, unsigned int Color);
int DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, const unsigned int Color)
{
    VECTOR p1 = { Pos1.x, Pos1.y, Pos1.z };	// 構造体 初期化子リスト
    VECTOR p2 = { Pos2.x, Pos2.y, Pos2.z };	// 構造体 初期化子リスト

    return DrawLine3D(p1, p2, Color);
}

// カメラの位置と姿勢の設定
// DxLib => int SetCameraPositionAndTargetAndUpVec(VECTOR Position, VECTOR Target, VECTOR Up);
int SetCameraPositionAndTargetAndUpVec(
    const Vector3& cameraPosition,	// カメラの位置
    const Vector3& cameraTarget,	// カメラの注視点
    const Vector3& cameraUp			// カメラの上の向き
)
{
    VECTOR position = { cameraPosition.x, cameraPosition.y, cameraPosition.z };
    VECTOR target = { cameraTarget.x, cameraTarget.y, cameraTarget.z };
    VECTOR up = { cameraUp.x, cameraUp.y, cameraUp.z };

    return SetCameraPositionAndTargetAndUpVec(position, target, up);
}

// モデルの座標変換用行列をセットする
// DxLib => int MV1SetMatrix(int MHandle, MATRIX Matrix);
int MV1SetMatrix(const int MHandle, const Matrix4 Matrix)
{
    MATRIX matrix;
    std::memcpy(&matrix, &Matrix, sizeof MATRIX);

    return MV1SetMatrix(MHandle, matrix);
}