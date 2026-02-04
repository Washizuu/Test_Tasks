import sys


def pinatas_maximization(pinatas: list[int]) -> int:
    n = len(pinatas)
    pinatas.insert(0,1)
    pinatas.append(1)

    temp_results = [[0] * (n + 2) for i in range(n + 2)]

    for length in range(1, n + 1):
        for i in range(1, n - length + 2):
            j = i + length - 1
            for k in range(i, j + 1):
                current_smash = pinatas[i - 1] * pinatas[k] * pinatas[j + 1]

                left_side = temp_results[i][k - 1]
                right_side = temp_results[k + 1][j]

                total = current_smash + left_side + right_side

                temp_results[i][j] = max(temp_results[i][j], total)

    return temp_results[1][n]

def main():
    if len(sys.argv) > 1:
        try:
            pinatas = [int(x) for x in sys.argv[1:]]
        except ValueError:
            print("Error: All inputs must be integers.")
            return
    else:
        print("Usage: python main.py <num1> <num2> ...")
        return

    result = pinatas_maximization(pinatas)
    print(result)

if __name__ == "__main__":
    main()