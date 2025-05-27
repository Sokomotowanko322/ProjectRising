#ifndef SINGLETON_H  
#define SINGLETON_H  

/// <summary>  
/// �V���O���g���e���v���[�g  
/// </summary>  
/// <typeparam name="T">�N���X</typeparam>  
template<typename T>  
class Singleton  
{  

public:  

/// <summary>  
/// �ÓI�ȃC���X�^���X�̎擾  
/// </summary>  
/// <returns>�N���X�̎Q��</returns>  
static inline T& GetInstance()  
{  
	static T instansce;  
	return instansce;  
}  

/// <summary>  
/// �f�X�g���N�^�O�̃��\�[�X�̉��  
/// </summary>  
/// <remarks>Dxlib_End()�O�ɉ������K�v������</remarks>  
virtual void Destroy(void) {};  

Singleton(const Singleton& singleton) = delete;  
Singleton& operator=(const Singleton& singleton) = delete;  

Singleton(Singleton&& singleton) = delete;  
Singleton& operator=(Singleton&& singleton) = delete;  

protected:  

Singleton() = default;  
virtual ~Singleton() = default;  

private:  

};  

#endif // SINGLETON_H