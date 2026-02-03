import csv
import sys
import os

def claim_analytics(day1_file, day2_file):
    first_day_history = dict()

    with open(day1_file, mode='r', encoding='utf-8') as f:
        reader = csv.reader(f)
        next(reader, None)

        for row in reader:
            user_id, product_id, _ = row

            if user_id not in first_day_history:
                first_day_history[user_id] = set()
            first_day_history[user_id].add(product_id)

    result_users = set()

    with open(day2_file, mode='r', encoding='utf-8') as f:
        reader = csv.reader(f)
        next(reader, None)

        for row in reader:
            user_id, product_id, _ = row
            if user_id in first_day_history and product_id not in first_day_history[user_id]:
                result_users.add(user_id)

    return list(result_users)


def main():

    if len(sys.argv) < 3:
        print("Error: No data sources specified..")
        print("Use: python main.py <day1.csv> <day2.csv>")
        sys.exit(1)

    file_path_1 = sys.argv[1]
    file_path_2 = sys.argv[2]

    if not os.path.exists(file_path_1):
        print(f"Error: File '{file_path_1}' not found.")
        sys.exit(1)
    if not os.path.exists(file_path_2):
        print(f"Error: File '{file_path_2}' not found.")
        sys.exit(1)

    try:
        users = claim_analytics(file_path_1, file_path_2)

        for user_id in users:
            print(user_id)

    except Exception as e:
        print(f"Error: {e}")


if __name__ == "__main__":
    main()