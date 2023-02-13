import sys
#This function will sort a given sequence of numbers
#if value in the list of numbers is not interger or float, program will exit
def sort_numbers(numbers):
    for value in numbers:
        check_value_int = isinstance(value, int)
        check_value_float = isinstance(value, float)

        if not check_value_int and not check_value_float:
            print("Error: " + str(value) + " is not integer or float.")
            sys.exit(1)

    numbers.sort()
    return numbers

def main():
    numbers = [3.9, 1.16, 4, 111, 5, 9, 2, 6, 5, 3, 5.0001]
    print(sort_numbers(numbers))


if __name__ == "__main__":
    main()
    sys.exit(0)
