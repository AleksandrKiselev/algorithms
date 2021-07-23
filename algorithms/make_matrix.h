#pragma once
#include <memory>
#include <gtest/gtest.h>


namespace matrixutils
{
	template<typename T>
	struct Deleter
	{
		Deleter(int count) : count_(count) {}
		~Deleter() = default;

		void operator()(T* matrix[])
		{
			for (int i = 0; i < count_; ++i)
			{
				delete[] matrix[i];
			}
			delete[] matrix;
		}

	private:
		int count_;
	};

	template<typename T>
	using MatrixUPtr = std::unique_ptr<T*[], Deleter<T>>;

	template<typename T>
	MatrixUPtr<T> makeMatrix(int n, int m)
	{
		MatrixUPtr<T> matrix(new T*[n], Deleter<T>(n));

		for (int i = 0; i < n; ++i)
		{
			matrix[i] = new T[m];
		}

		return matrix;
	}
}

TEST(MakeMatrixTest, SimpleTest) 
{
	auto matrix = matrixutils::makeMatrix<std::pair<int, int>>(3, 2);
	matrix[0][0] = std::make_pair(0, 0);
	matrix[0][1] = std::make_pair(0, 1);
	matrix[1][0] = std::make_pair(1, 0);
	matrix[1][1] = std::make_pair(1, 1);
	matrix[2][0] = std::make_pair(2, 0);
	matrix[2][1] = std::make_pair(2, 1);

	std::pair<int, int>** rawMatrix = matrix.get();

	ASSERT_EQ(rawMatrix[0][0], std::make_pair(0, 0));
	ASSERT_EQ(rawMatrix[0][1], std::make_pair(0, 1));
	ASSERT_EQ(rawMatrix[1][0], std::make_pair(1, 0));
	ASSERT_EQ(rawMatrix[1][1], std::make_pair(1, 1));
	ASSERT_EQ(rawMatrix[2][0], std::make_pair(2, 0));
	ASSERT_EQ(rawMatrix[2][1], std::make_pair(2, 1));
}
