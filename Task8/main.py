import heapq
import sys


def students_capital(n: int, c: int, prices_array: list[int], gains_array: list[int]) -> int:
    price_gain_list = sorted(zip(prices_array, gains_array), key=lambda pair: pair[0])

    index = 0
    available_laptops = []
    for i in range(n):
        while index < len(price_gain_list) and price_gain_list[index][0] <= c:
            gain = price_gain_list[index][1]
            heapq.heappush(available_laptops, -gain)
            index += 1

        if not available_laptops:
            break

        max_profit = -heapq.heappop(available_laptops)

        c += max_profit

    return c

def main():
    n = int(input("Maximum number of laptops to buy (N): "))
    c = int(input("Initial Capital (C): "))
    prices_array = list(map(int, input("Enter the prices array(space separated): ").split()))
    gains_array = list(map(int, input("Enter the gains array(space separated): ").split()))

    if len(prices_array) != len(gains_array):
        print("Error: The number of prices must match the number of gains.")
        sys.exit(1)

    result = students_capital(n, c, prices_array, gains_array)
    print(f"Capital at the end of the summer: {result}")

if __name__ == "__main__":
    main()