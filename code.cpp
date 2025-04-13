#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct State {
    int cost;           // Текущая суммарная стоимость
    int coupons_left;   // Количество доступных купонов
    vector<int> used;   // Дни, когда использовались купоны
};

int main() {
    int n;
    cin >> n;
    vector<int> prices(n);
    for (int i = 0; i < n; ++i) {
        cin >> prices[i];
    }

    // Инициализация DP таблицы: день × купоны → состояние
    vector<vector<State>> dp(n + 1, vector<State>(n + 1, {INT_MAX, 0, {}}));
    dp[0][0] = {0, 0, {}};

    for (int day = 1; day <= n; ++day) {
        int price = prices[day - 1];
        for (int coupons = 0; coupons <= n; ++coupons) {
            if (dp[day - 1][coupons].cost == INT_MAX) continue;

            // Вариант 1: Покупаем обед и получаем купон (если цена > 100)
            State buy = dp[day - 1][coupons];
            buy.cost += price;
            if (price > 100) buy.coupons_left++;
            
            if (buy.cost < dp[day][buy.coupons_left].cost || 
               (buy.cost == dp[day][buy.coupons_left].cost && buy.coupons_left > dp[day][buy.coupons_left].coupons_left)) {
                dp[day][buy.coupons_left] = buy;
            }

            // Вариант 2: Используем купон (если есть)
            if (coupons > 0) {
                State use = dp[day - 1][coupons];
                use.coupons_left--;
                use.used.push_back(day);
                
                if (use.cost < dp[day][use.coupons_left].cost || 
                   (use.cost == dp[day][use.coupons_left].cost && use.coupons_left > dp[day][use.coupons_left].coupons_left)) {
                    dp[day][use.coupons_left] = use;
                }
            }
        }
    }

    // Поиск оптимального решения
    State best = {INT_MAX, 0, {}};
    for (int coupons = 0; coupons <= n; ++coupons) {
        if (dp[n][coupons].cost < best.cost || 
           (dp[n][coupons].cost == best.cost && coupons > best.coupons_left)) {
            best = dp[n][coupons];
        }
    }

    // Сортировка дней использования купонов
    sort(best.used.begin(), best.used.end());

    // Вывод результатов
    cout << best.cost << "\n";
    cout << best.coupons_left << " " << best.used.size() << "\n";
    for (int day : best.used) {
        cout << day << "\n";
    }

    return 0;
}
