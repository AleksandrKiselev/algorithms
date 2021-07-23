#pragma once
#include <memory>
#include <gtest/gtest.h>


namespace matrixutils
{
	template<typename T>
	struct Deleter
	{
		Deleter(int count) : m_Count(count) {}
		~Deleter() = default;

		void operator()(T* matrix[])
		{
			for (int i = 0; i < m_Count; ++i)
			{
				delete[] matrix[i];
			}
			delete[] matrix;
		}

	private:
		int m_Count;
	};

	template<typename T>
	using MatrixUPtr = std::unique_ptr<T* [], Deleter<T>>;

	template<typename T>
	MatrixUPtr<T> makeMatrix(int n, int m)
	{
		MatrixUPtr<T> matrix(new T * [n], Deleter<T>(n));

		for (int i = 0; i < n; ++i)
		{
			matrix[i] = new T[m];
		}

		return matrix;
	}
}

TEST(MakeMatrixTest, BasicAssertion) 
{
	{
		auto matrix = matrixutils::makeMatrix<double>(3, 2);
		matrix[0][0] = 0;
		matrix[0][1] = 1;
		matrix[1][0] = 10;
		matrix[1][1] = 11;
		matrix[2][0] = 20;
		matrix[2][1] = 21;

		double** M = matrix.get();

		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 2; ++j) {
				std::cout << M[i][j] << std::endl; 
			}
		}

	}
}
