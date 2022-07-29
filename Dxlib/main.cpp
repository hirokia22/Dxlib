#include "Vector3.h"
#include "Matrix4.h"
#include <DxLib.h>
#include <cstring>  // memcpy

// ��(Sphere)�̕`��
// DxLib => int DrawSphere3D(VECTOR CenterPos, float r, int DivNum, unsigned int DifColor, unsigned int SpcColor, int FillFlag);
int DrawSphere3D(const Vector3& CenterPos, const float r, const int DivNum, const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag);

// �~��(Cone)�̕`��
// DxLib => int DrawCone3D( VECTOR TopPos, VECTOR BottomPos, float r, int DivNum, unsigned int DifColor, unsigned int SpcColor, int FillFlag);
int DrawCone3D(const Vector3& TopPos, const Vector3& BottomPos, const float r,
    const int DivNum, const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag);

// �����̕`��
// DxLib => int DrawLine3D(VECTOR Pos1, VECTOR Pos2, unsigned int Color);
int DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, const unsigned int Color);

// �J�����̈ʒu�Ǝp���̐ݒ�
// DxLib => int SetCameraPositionAndTargetAndUpVec(VECTOR Position, VECTOR Target, VECTOR Up);
int SetCameraPositionAndTargetAndUpVec(
    const Vector3& cameraPosition,    // �J�����̈ʒu
    const Vector3& cameraTarget,      // �J�����̒����_
    const Vector3& cameraUp		    // �J�����̏�̌���
);

// ���f���̍��W�ϊ��p�s����Z�b�g����
// DxLib => int MV1SetMatrix(int MHandle, MATRIX Matrix);
int MV1SetMatrix(const int MHandle, const Matrix4 Matrix);


// �֐��v���g�^�C�v�錾
void DrawAxis3D(const float length);	// x,y,z ���̕`��
void DrawKeyOperation();                // �L�[����̕`��

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // DxLib ������
    const int WindowWidth = 1600;
    const int WindowHeight = 900;

    ChangeWindowMode(TRUE);	    					// �E�C���h�E���[�h�ɂ���
    SetGraphMode(WindowWidth, WindowHeight, 32);	// ��ʃ��[�h�̃Z�b�g
    SetBackgroundColor(0, 0, 64);       // �w�i�F RGB
    if (DxLib_Init() == -1) return -1;	// DxLib ����������
    SetDrawScreen(DX_SCREEN_BACK);		// �`����ʂ𗠉�ʂɃZ�b�g

    SetUseZBuffer3D(TRUE);      // �y�o�b�t�@��L���ɂ���
    SetWriteZBuffer3D(TRUE);    // �y�o�b�t�@�ւ̏������݂�L���ɂ���

    // �J����������
    Vector3 cameraPosition(50.0f, 50.0f, -400.0f);
    Vector3 cameraTarget(0.0f, 0.0f, 0.0f);
    Vector3 cameraUp(0.0f, 1.0f, 0.0f);

    //    �N���b�v��  ��     ��
    SetCameraNearFar(1.0f, 10000.0f);		//�J�����̗L���͈͂�ݒ�
    SetCameraScreenCenter(WindowWidth / 2.0f, WindowHeight / 2.0f);  // ��ʂ̒��S���J�����̒��S�ɍ��킹��
    SetCameraPositionAndTargetAndUpVec(cameraPosition, cameraTarget, cameraUp);

    // �A�v���Ŏg�p����ϐ�
    int model = MV1LoadModel("fighter.mqo");

    // xyz���̉�]�p�x
    const float ROT_UNIT = 0.01f;
    float rotX = 0.0f;
    float rotY = 0.0f;
    float rotZ = 0.0f;

    // �Q�[�����[�v
    while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
    {
        // �X�V ---------------------------
        if (CheckHitKey(KEY_INPUT_A))   rotY += ROT_UNIT;
        if (CheckHitKey(KEY_INPUT_D))   rotY -= ROT_UNIT;

        if (CheckHitKey(KEY_INPUT_W))   rotX += ROT_UNIT;
        if (CheckHitKey(KEY_INPUT_S))   rotX -= ROT_UNIT;

        if (CheckHitKey(KEY_INPUT_E))   rotZ += ROT_UNIT;
        if (CheckHitKey(KEY_INPUT_Z))   rotZ -= ROT_UNIT;

        // [R] �Ń��Z�b�g
        if (CheckHitKey(KEY_INPUT_R))
        {
            rotX = rotY = rotZ = 0;
        }

        // �e��ϊ��s��̌v�Z
        Matrix4 matScale = scale(Vector3(5.1f, 5.1f, 5.1f));	// ���f���̊g�嗦

        Matrix4 matRotX = rotateX(rotX);
        Matrix4 matRotY = rotateY(rotY);
        Matrix4 matRotZ = rotateZ(rotZ);

        Matrix4 matRot = matRotX * matRotY * matRotZ;

        Matrix4 matWorld = matScale * matRot;

        MV1SetMatrix(model, matWorld);

        // �`�� ---------------------------
        ClearDrawScreen();		// ��ʂ�����
        DrawAxis3D(200.0f);     // xyz���̕`��

        // ���f���̕`��
        MV1DrawModel(model);

        // �L�[����̕`��
        DrawKeyOperation();

        // �t���b�v����
        ScreenFlip();
    }
    DxLib::DxLib_End(); 	// DxLib �I������
    return 0; 	// �\�t�g�̏I��
}

// xyz���̕`��
void DrawAxis3D(const float length)
{
    // ���̐��̕`��
    DrawLine3D(Vector3(-length, 0, 0), Vector3(+length, 0, 0), GetColor(255, 0, 0));	// x��
    DrawLine3D(Vector3(0, -length, 0), Vector3(0, +length, 0), GetColor(0, 255, 0));	// y��
    DrawLine3D(Vector3(0, 0, -length), Vector3(0, 0, +length), GetColor(0, 0, 255));	// z��

    // ���̐�̕`��
    const float coneSize = 10.0f;
    DrawCone3D(Vector3(length, 0, 0), Vector3(length - coneSize, 0, 0), coneSize / 2, 16, GetColor(255, 0, 0), GetColor(255, 255, 255), TRUE);
    DrawCone3D(Vector3(0, length, 0), Vector3(0, length - coneSize, 0), coneSize / 2, 16, GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);
    DrawCone3D(Vector3(0, 0, length), Vector3(0, 0, length - coneSize), coneSize / 2, 16, GetColor(0, 0, 255), GetColor(255, 255, 255), TRUE);
}

// �L�[����̕`��
void DrawKeyOperation()
{
    const unsigned white = GetColor(255, 255, 255);

    DrawFormatString(10, 20 * 1, white, "  [W][E][R]  R : ���Z�b�g");
    DrawFormatString(10, 20 * 2, white, "[A][S][D]    AD: y���܂��̉�]");
    DrawFormatString(10, 20 * 3, white, " [Z]         WS: x���܂��̉�]");
    DrawFormatString(10, 20 * 4, white, "             EZ: z���܂��̉�]");
}

// Vector3�^�𗘗p���₷�����邽�߂� �I�[�o�[���[�h�֐��Q

// ���̕`��
// DxLib => int DrawSphere3D(VECTOR CenterPos, float r, int DivNum, unsigned int DifColor, unsigned int SpcColor, int FillFlag);
int DrawSphere3D(const Vector3& CenterPos, const float r, const int DivNum, const unsigned int DifColor, const unsigned int SpcColor, const int FillFlag)
{
    VECTOR centerPos = { CenterPos.x, CenterPos.y, CenterPos.z };	// �\���� �������q���X�g

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

// �����̕`��
// DxLib => int DrawLine3D(VECTOR Pos1, VECTOR Pos2, unsigned int Color);
int DrawLine3D(const Vector3& Pos1, const Vector3& Pos2, const unsigned int Color)
{
    VECTOR p1 = { Pos1.x, Pos1.y, Pos1.z };	// �\���� �������q���X�g
    VECTOR p2 = { Pos2.x, Pos2.y, Pos2.z };	// �\���� �������q���X�g

    return DrawLine3D(p1, p2, Color);
}

// �J�����̈ʒu�Ǝp���̐ݒ�
// DxLib => int SetCameraPositionAndTargetAndUpVec(VECTOR Position, VECTOR Target, VECTOR Up);
int SetCameraPositionAndTargetAndUpVec(
    const Vector3& cameraPosition,	// �J�����̈ʒu
    const Vector3& cameraTarget,	// �J�����̒����_
    const Vector3& cameraUp			// �J�����̏�̌���
)
{
    VECTOR position = { cameraPosition.x, cameraPosition.y, cameraPosition.z };
    VECTOR target = { cameraTarget.x, cameraTarget.y, cameraTarget.z };
    VECTOR up = { cameraUp.x, cameraUp.y, cameraUp.z };

    return SetCameraPositionAndTargetAndUpVec(position, target, up);
}

// ���f���̍��W�ϊ��p�s����Z�b�g����
// DxLib => int MV1SetMatrix(int MHandle, MATRIX Matrix);
int MV1SetMatrix(const int MHandle, const Matrix4 Matrix)
{
    MATRIX matrix;
    std::memcpy(&matrix, &Matrix, sizeof MATRIX);

    return MV1SetMatrix(MHandle, matrix);
}