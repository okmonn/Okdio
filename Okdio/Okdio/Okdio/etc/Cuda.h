#pragma once
#include <iostream>
#include <cuda_runtime.h>

#pragma comment(lib, "cudart_static.lib")

namespace cuda
{
	// �G���[�`�F�b�N
	void Check(const cudaError_t& error)
	{
		if (error != cudaSuccess)
		{
			std::cout << "Error�F" << __FILE__ << "&" << __LINE__ << std::endl;
			std::cout << "Code�F" << error << std::endl;
			std::cout << "Reason�F" << cudaGetErrorString(error) << std::endl;
		}
	}

	// GPU�������m��
	template <typename T>
	cudaError_t Malloc(T** addr, const size_t& size) {
		return cudaMalloc(&(*addr), size);
	}

	// GPU�������̃Z�b�g
	cudaError_t Set(void* addr, const int& data, const size_t& size) {
		return cudaMemset(addr, data, size);
	}

	// �������R�s�[
	template <typename T>
	cudaError_t Copy(T** addr, void* data, const size_t& size, cudaMemcpyKind& type) {
		return cudaMemcpy((void*)&(*addr), data, size, type);
	}

	// �J�[�l������
	cudaError_t Sync(void) {
		return cudaDeviceSynchronize();
	}

	// GPU���������
	template <typename T>
	cudaError_t Free(T** addr) {
		return cudaFree((void*)&(*addr));
	}
}
