#pragma once

namespace Engine
{
	template<typename T>
	void Safe_Delete(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete Pointer;
			Pointer = nullptr;
		}
	}

	template<typename T>
	void Safe_Delete_Array(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete[] Pointer;
			Pointer = nullptr;
		}
	}

	template<typename T>
	unsigned int Safe_AddRef(T& Instance)
	{
		unsigned int iRefCnt = { 0 };

		if (nullptr != Instance)
			iRefCnt = Instance->AddRef();

		return iRefCnt;
	}

	template<typename T>
	unsigned int Safe_Release(T& Instance)
	{
		unsigned int iRefCnt = { 0 };

		if (nullptr != Instance)
		{
			iRefCnt = Instance->Release();
			if (0 == iRefCnt)
				Instance = nullptr;
		}
		return iRefCnt;
	}

	inline float Lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}

	template<typename... Args>
	class Delegate
	{
	public:
		// 1) 내부에서 호출될 함수 시그니처
		using FuncPtr = void(*)(void*, Args...);
		void* m_target = nullptr;  // 호출할 객체(this)
		FuncPtr m_invoke = nullptr;  // 실제 호출할 함수
	public:
		Delegate() = default;

		// 2) 바인딩: T 객체의 멤버 함수와 this를 캡처
		template<typename T, void(T::* method)(Args...)>
		void Bind(T* instance) {
			m_target = instance;
			m_invoke = [](void* obj, Args... args) {
				(static_cast<T*>(obj)->*method)(args...);
				};
		}

		// 3) 실행: 저장된 객체와 멤버 함수를 호출
		void Execute(Args... args) const {
			if (m_invoke)
				m_invoke(m_target, args...);
		}

		// 유효성 확인
		explicit operator bool() const { return m_invoke != nullptr; }

		void* Get_Instance() const { return m_target; }
	};
}
