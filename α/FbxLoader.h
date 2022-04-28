#pragma once

#include "fbxsdk.h"
#include "Fbx_Model.h"
#include <d3d12.h>
#include <d3dx12.h>
#include <string>

class FbxLoader
{
private://�G�C���A�X
	//std::���ȗ�
	using string = std::string;

public://�萔
	//���f���i�[���[�g�p�X
	static const string baseDirectory;
	//�e�N�X�`�����Ȃ��ꍇ�̕W���e�N�X�`���t�@�C����
	static const string defaultTextureFileName;
public:
	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static FbxLoader* GetInstance();

private:
	// private�ȃR���X�g���N�^�i�V���O���g���p�^�[���j
	FbxLoader() = default;
	// private�ȃf�X�g���N�^�i�V���O���g���p�^�[���j
	~FbxLoader() = default;
	// �R�s�[�R���X�g���N�^���֎~�i�V���O���g���p�^�[���j
	FbxLoader(const FbxLoader& obj) = delete;
	// �R�s�[������Z�q���֎~�i�V���O���g���p�^�[���j
	void operator=(const FbxLoader& obj) = delete;

public://�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="device">D3D12�f�o�C�X</param>
	void Initialize(ID3D12Device* device);

	/// <summary>
	/// ��n��
	/// </summary>
	void Finalize();

	/// <summary>
	/// �t�@�C������FBX���f���̓ǂݍ���
	/// </summary>
	/// <param name="modelName">���f����</param>
	//void LoadModelFromFile(const string& modelName);
	Fbx_Model* LoadModelFromFile(const string& modelName);

	/// <summary>
	/// �ċA�I�Ƀm�[�h�\�������
	/// </summary>
	/// <param name="fbx_model">�ǂݍ��ݐ惂�f���I�u�W�F�N�g</param>
	/// <param name="fbxNode">��͑Ώۂ̃m�[�h</param>
	void ParseNodeRecursive(Fbx_Model* fbx_model, FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// ���b�V���ǂݎ��
	/// </summary>
	/// <param name="fbx_model">�ǂݍ��ݐ惂�f���I�u�W�F�N�g</param>
	/// <param name="fbxNode">��͑Ώۂ̃m�[�h</param>
	void ParseMesh(Fbx_Model* fbx_model, FbxNode* fbxNode);

	/// <summary>
	/// FBX�̍s���XMMatrix�ɕϊ�
	/// </summary>
	/// <param name="dst">�������ݐ�</param>
	/// <param name="src">���ƂȂ�FBX�s��</param>
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);

	//���_���ǂݎ��
	void ParseMeshVertices(Fbx_Model* fbx_model, FbxMesh* fbxMesh);
	//�ʏ��ǂݎ��
	void ParseMeshFaces(Fbx_Model* fbx_model, FbxMesh* fbxMesh);
	//�}�e���A���ǂݎ��
	void ParseMaterial(Fbx_Model* fbx_model, FbxNode* fbxNode);
	//�e�N�X�`���ǂݍ���
	void LoadTexture(Fbx_Model* fbx_model, const std::string& fullpath);
	//�f�B���N�g�����܂񂾃t�@�C���p�X����t�@�C�����𒊏o����
	std::string ExtractFileName(const std::string& path);
	//�X�L�j���O���̓ǂݎ��
	void ParseSkin(Fbx_Model* fbx_model, FbxMesh* fbxMesh);

private:
	//D3D12�f�o�C�X
	ID3D12Device* device = nullptr;
	//FBX�}�l�[�W��
	FbxManager* fbxManager = nullptr;
	//FBX�C���|�[�^
	FbxImporter* fbxImporter = nullptr;

};