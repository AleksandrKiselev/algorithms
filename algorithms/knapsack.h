#pragma once
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <stdexcept>

namespace knapsack
{

/**
 * Задача о рюкзаке (0/1 Knapsack Problem)
 * 
 * ФОРМУЛИРОВКА ЗАДАЧИ:
 * Есть рюкзак вместимостью W (максимальный вес) и набор из n предметов.
 * Каждый предмет i имеет вес weight[i] и ценность value[i].
 * 
 * Нужно выбрать подмножество предметов так, чтобы:
 *   1. Суммарный вес не превышал W
 *   2. Суммарная ценность была максимальной
 * 
 * ОГРАНИЧЕНИЕ: каждый предмет можно взять только один раз (0/1 - либо берем, либо нет)
 * 
 * ПРИМЕР:
 *   Вместимость: W = 10
 *   Предметы: [(5,10), (4,40), (6,30), (3,50)]  // (вес, ценность)
 *   Оптимальное решение: предметы 2 и 4
 *   Вес: 4 + 3 = 7 <= 10, Ценность: 40 + 50 = 90
 */

/**
 * Структура для представления предмета
 */
struct Item
{
	int weight;  // Вес предмета
	int value;   // Ценность предмета

	Item(int w, int v) : weight(w), value(v)
	{
		if (weight < 0 || value < 0)
		{
			throw std::invalid_argument("Weight and value must be non-negative");
		}
	}
};

/**
 * РЕШЕНИЕ МЕТОДОМ ДИНАМИЧЕСКОГО ПРОГРАММИРОВАНИЯ
 * 
 * ПОЧЕМУ DP ПОДХОДИТ:
 * 
 * 1. ПЕРЕКРЫВАЮЩИЕСЯ ПОДЗАДАЧИ (Overlapping Subproblems):
 *    Подзадача определяется параметрами (i, w):
 *      - i: количество рассмотренных предметов (первые i предметов)
 *      - w: максимальный доступный вес (от 0 до W)
 *    
 *    Результат: dp[i][w] - максимальная ценность с первыми i предметами при доступном весе w.
 *    
 *    При рекурсивном решении одна подзадача (i, w) встречается в разных ветвях дерева.
 *    Например, подзадача (3, 5) может появиться как:
 *      - Взяли предметы 1 и 2, не взяли 3
 *      - Не взяли 1, взяли предметы 2 и 3
 *    DP вычисляет каждую подзадачу один раз и сохраняет результат.
 * 
 * 2. ОПТИМАЛЬНАЯ ПОДСТРУКТУРА (Optimal Substructure):
 *    Оптимальное решение содержит оптимальные решения подзадач.
 *    Зная оптимальное решение для (i, w), можем построить решение для (i+1, w).
 * 
 * АЛГОРИТМ:
 * 
 * Таблица dp[i][w]:
 *   - i: количество предметов (0..n)
 *   - w: максимальный доступный вес (0..W)
 *   - dp[i][w]: максимальная ценность с первыми i предметами при доступном весе w
 * 
 * Базовые случаи:
 *   dp[0][w] = 0  (нет предметов)
 *   dp[i][0] = 0  (нет доступного веса)
 * 
 * Рекуррентное соотношение:
 *   dp[i][w] = max(
 *       dp[i-1][w],                          // не берем предмет i
 *       dp[i-1][w - weight[i]] + value[i]    // берем предмет i (если weight[i] <= w)
 *   )
 * 
 * ПРИМЕР:
 *   Предметы: [(5,10), (4,40), (6,30), (3,50)], W = 10
 * 
 *            w=0  w=1  w=2  w=3  w=4  w=5  w=6  w=7  w=8  w=9  w=10
 *   i=0       0    0    0    0    0    0    0    0    0    0    0
 *   i=1 (5,10)0    0    0    0    0   10   10   10   10   10   10
 *   i=2 (4,40)0    0    0    0   40   40   40   40   40   50   50
 *   i=3 (6,30)0    0    0    0   40   40   40   40   40   50   70
 *   i=4 (3,50)0    0    0   50   50   50   50   90   90   90   90
 * 
 *   Ответ: dp[4][10] = 90 (предметы с весом 4 и 3, использовано 7 из 10)
 * 
 * СЛОЖНОСТЬ: O(n * W) по времени, O(n * W) по памяти (O(W) при оптимизации)
 */

/**
 * Решает задачу о рюкзаке методом динамического программирования
 * 
 * @param items Вектор предметов (вес и ценность)
 * @param capacity Вместимость рюкзака
 * @return Максимальная ценность, которую можно унести в рюкзаке
 */
int solve_knapsack(const std::vector<Item>& items, int capacity)
{
	if (capacity < 0)
	{
		throw std::invalid_argument("Capacity must be non-negative");
	}

	int n = static_cast<int>(items.size());

	if (n == 0 || capacity == 0)
	{
		return 0;
	}

	// dp[i][w] = максимальная ценность с первыми i предметами при доступном весе w
	std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));

	for (int i = 1; i <= n; ++i)
	{
		const Item& item = items[i - 1];

		for (int w = 0; w <= capacity; ++w)
		{
			// Не берем предмет i
			dp[i][w] = dp[i - 1][w];

			// Берем предмет i (если помещается)
			if (item.weight <= w)
			{
				dp[i][w] = std::max(dp[i][w], dp[i - 1][w - item.weight] + item.value);
			}
		}
	}

	return dp[n][capacity];
}

/**
 * Оптимизированная версия с использованием одного массива
 * 
 * Для вычисления dp[i][w] нужны только значения из строки i-1,
 * поэтому используем один массив dp[w] вместо таблицы.
 * 
 * ВАЖНО: обрабатываем w в порядке убывания (от capacity к item.weight).
 * Это гарантирует, что dp[w - item.weight] еще не обновлено для текущего предмета.
 * Если идти от меньшего к большему, один предмет может быть взят дважды.
 * 
 * СЛОЖНОСТЬ: O(n * W) по времени, O(W) по памяти
 */
int solve_knapsack_optimized(const std::vector<Item>& items, int capacity)
{
	if (capacity < 0)
	{
		throw std::invalid_argument("Capacity must be non-negative");
	}

	int n = static_cast<int>(items.size());

	if (n == 0 || capacity == 0)
	{
		return 0;
	}

	// dp[w] = максимальная ценность при доступном весе w
	std::vector<int> dp(capacity + 1, 0);

	for (const Item& item : items)
	{
		// Обрабатываем w в порядке убывания (от capacity к item.weight)
		// чтобы dp[w - item.weight] еще не содержало текущий предмет
		for (int w = capacity; w >= item.weight; --w)
		{
			dp[w] = std::max(dp[w], dp[w - item.weight] + item.value);
		}
	}

	return dp[capacity];
}

/**
 * Восстанавливает набор предметов, дающих оптимальное решение
 * 
 * Алгоритм: идем по таблице обратно (от dp[n][W] к dp[0][0]).
 * Если dp[i][w] != dp[i-1][w], предмет i был взят, переходим к dp[i-1][w - weight[i]].
 * Иначе предмет не был взят, переходим к dp[i-1][w].
 * 
 * @param items Вектор предметов
 * @param capacity Вместимость рюкзака
 * @return Вектор индексов предметов, которые нужно взять
 */
std::vector<int> get_selected_items(const std::vector<Item>& items, int capacity)
{
	if (capacity < 0)
	{
		throw std::invalid_argument("Capacity must be non-negative");
	}

	int n = static_cast<int>(items.size());

	if (n == 0 || capacity == 0)
	{
		return {};
	}

	// Строим таблицу
	std::vector<std::vector<int>> dp(n + 1, std::vector<int>(capacity + 1, 0));

	for (int i = 1; i <= n; ++i)
	{
		const Item& item = items[i - 1];
		for (int w = 0; w <= capacity; ++w)
		{
			dp[i][w] = dp[i - 1][w];
			if (item.weight <= w)
			{
				dp[i][w] = std::max(dp[i][w], dp[i - 1][w - item.weight] + item.value);
			}
		}
	}

	// Восстанавливаем решение
	std::vector<int> selected;
	int w = capacity;

	for (int i = n; i > 0; --i)
	{
		if (dp[i][w] != dp[i - 1][w])
		{
			selected.push_back(i - 1);
			w -= items[i - 1].weight;
		}
	}

	std::reverse(selected.begin(), selected.end());

	return selected;
}

// ============================================================================
// ТЕСТЫ
// ============================================================================

TEST(KnapsackTest, EmptyKnapsack)
{
	std::vector<Item> items;
	EXPECT_EQ(solve_knapsack(items, 10), 0);
	EXPECT_EQ(solve_knapsack_optimized(items, 10), 0);
}

TEST(KnapsackTest, ZeroCapacity)
{
	std::vector<Item> items = {Item(5, 10), Item(3, 5)};
	EXPECT_EQ(solve_knapsack(items, 0), 0);
	EXPECT_EQ(solve_knapsack_optimized(items, 0), 0);
}

TEST(KnapsackTest, SingleItemFits)
{
	std::vector<Item> items = {Item(5, 10)};
	EXPECT_EQ(solve_knapsack(items, 10), 10);
	EXPECT_EQ(solve_knapsack_optimized(items, 10), 10);
}

TEST(KnapsackTest, SingleItemDoesNotFit)
{
	std::vector<Item> items = {Item(15, 10)};
	EXPECT_EQ(solve_knapsack(items, 10), 0);
	EXPECT_EQ(solve_knapsack_optimized(items, 10), 0);
}

TEST(KnapsackTest, TwoItemsBothFit)
{
	std::vector<Item> items = {Item(3, 10), Item(4, 15)};
	EXPECT_EQ(solve_knapsack(items, 10), 25);
	EXPECT_EQ(solve_knapsack_optimized(items, 10), 25);
}

TEST(KnapsackTest, TwoItemsOnlyOneFits)
{
	std::vector<Item> items = {Item(3, 10), Item(8, 15)};
	EXPECT_EQ(solve_knapsack(items, 7), 10);
	EXPECT_EQ(solve_knapsack_optimized(items, 7), 10);
}

TEST(KnapsackTest, ClassicExample)
{
	// Предметы: [(5,10), (4,40), (6,30), (3,50)], W = 10
	// Оптимальное решение: предметы 1 и 3 (вес 7, ценность 90)
	std::vector<Item> items = {
		Item(5, 10),
		Item(4, 40),
		Item(6, 30),
		Item(3, 50)
	};

	EXPECT_EQ(solve_knapsack(items, 10), 90);
	EXPECT_EQ(solve_knapsack_optimized(items, 10), 90);

	std::vector<int> selected = get_selected_items(items, 10);
	EXPECT_EQ(selected.size(), 2);
	EXPECT_EQ(selected[0], 1);
	EXPECT_EQ(selected[1], 3);

	int total_weight = items[selected[0]].weight + items[selected[1]].weight;
	EXPECT_EQ(total_weight, 7);
}

TEST(KnapsackTest, AllItemsFit)
{
	std::vector<Item> items = {Item(2, 5), Item(3, 10), Item(1, 3)};
	EXPECT_EQ(solve_knapsack(items, 10), 18);
	EXPECT_EQ(solve_knapsack_optimized(items, 10), 18);
}

TEST(KnapsackTest, GreedyWouldFail)
{
	// Пример, где жадный алгоритм не дает оптимального решения
	// Предметы: [(3,4), (2,3), (2,3)], W = 4
	// Жадный (по ценности/весу) возьмет один предмет (ценность 3)
	// Оптимально: предметы 2 и 3 (ценность 6)
	std::vector<Item> items = {
		Item(3, 4),
		Item(2, 3),
		Item(2, 3)
	};
	EXPECT_EQ(solve_knapsack(items, 4), 6);
	EXPECT_EQ(solve_knapsack_optimized(items, 4), 6);
}

TEST(KnapsackTest, GetSelectedItems)
{
	std::vector<Item> items = {
		Item(5, 10),
		Item(4, 40),
		Item(6, 30),
		Item(3, 50)
	};

	std::vector<int> selected = get_selected_items(items, 10);
	EXPECT_EQ(selected.size(), 2);
	EXPECT_EQ(selected[0], 1);
	EXPECT_EQ(selected[1], 3);
	EXPECT_EQ(items[selected[0]].value + items[selected[1]].value, 90);
}

TEST(KnapsackTest, InvalidInput)
{
	std::vector<Item> items = {Item(5, 10)};
	
	EXPECT_THROW(solve_knapsack(items, -1), std::invalid_argument);
	EXPECT_THROW(solve_knapsack_optimized(items, -1), std::invalid_argument);
	EXPECT_THROW(get_selected_items(items, -1), std::invalid_argument);
	
	EXPECT_THROW(Item(-1, 10), std::invalid_argument);
	EXPECT_THROW(Item(5, -1), std::invalid_argument);
}

} // namespace knapsack

