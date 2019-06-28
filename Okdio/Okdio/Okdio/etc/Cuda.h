#pragma once
#include <iostream>
#include <cuda_runtime.h>

#pragma comment(lib, "cudart_static.lib")

namespace cuda
{
	// エラーチェック
	void Check(const cudaError_t& error)
	{
		if (error != cudaSuccess)
		{
			std::cout << "Error：" << __FILE__ << "&" << __LINE__ << std::endl;
			std::cout << "Code：" << error << std::endl;
			std::cout << "Reason：" << cudaGetErrorString(error) << std::endl;
		}
	}

	// GPUメモリ確保
	template <typename T>
	cudaError_t Malloc(T** addr, const size_t& size) {
		return cudaMalloc(&(*addr), size);
	}

	// GPUメモリのセット
	cudaError_t Set(void* addr, const int& data, const size_t& size) {
		return cudaMemset(addr, data, size);
	}

	// メモリコピー
	template <typename T>
	cudaError_t Copy(T** addr, void* data, const size_t& size, cudaMemcpyKind& type) {
		return cudaMemcpy((void*)&(*addr), data, size, type);
	}

	// カーネル同期
	cudaError_t Sync(void) {
		return cudaDeviceSynchronize();
	}

	// GPUメモリ解放
	template <typename T>
	cudaError_t Free(T** addr) {
		return cudaFree((void*)&(*addr));
	}
}
