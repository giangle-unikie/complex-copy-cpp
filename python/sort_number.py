import sys


#This function will sort a given sequence of numbers
#if value in the list of numbers is not interger or float, program will exit
def sort_numbers(numbers):
    for value in numbers:
        value_is_number = value.replace('.', '', 1).isdigit()
        if not value_is_number and not (value.startswith("-") and value[1:].isdigit()):
            print("Error:", value, "is not integer or float.")
            sys.exit(1)

    numbers.sort()
    return numbers


def main():
    numbers_list = input("Enter the numbers separated by space: ").strip().split()
    print(sort_numbers(numbers_list))


if __name__ == "__main__":
    main()
    sys.exit(0)
