def integer_to_binary(number, length):
    binary_str = format(number, 'b').zfill(length)
    return binary_str


def pad_left_zeroes(binary_number, nr_zeroes):
    return '0' * nr_zeroes + binary_number[:-nr_zeroes]

def shuffle(start_int, stop_int, nr_floors):
    start = integer_to_binary(start_int, nr_floors)
    stop = integer_to_binary(stop_int, nr_floors)

    start_len = len(start)
    stop_len = len(stop)

    if start_len > stop_len:
        stop = pad_left_zeroes(stop, start_len - stop_len)
    elif stop_len > start_len:
        start = pad_left_zeroes(start, stop_len - start_len)

    for i in range(len(start)):
        print(f"Floor {len(start) - i - 1}: ", end='')
        if start[i] == stop[i]:
            print("direct connection")
        else:
            print("inverse connection")

def number_of_figures(number):
    figures = 1 if number == 0 else 0
    while number != 0:
        number //= 10
        figures += 1
    return figures

def print_binary(number, length):
    binary_str = format(number, 'b')
    print(binary_str.zfill(length), end='')

def print_map(size, log_size):
    print("Index  Start  Stop")
    for i in range(size + 1):
        print(f"{i} {' ' * (5 - number_of_figures(i))}", end='')
        print_binary(i, log_size)
        print("   ", end='')
        print_binary(i, log_size)
        print()

if __name__ == "__main__":
    nr_floors = 0
    nr_max = 1
    nr_chosen_start = 0
    nr_chosen_stop = 0

    while nr_floors <= 0:
        nr_floors = int(input("Enter network dimension (number of floors)\n"))

    nr_max = (nr_max << nr_floors) - 1

    print(f"The chosen network has {nr_floors} floors and accepts values "
          f"between 0 and {nr_max}\n")

    print_map(nr_max, nr_floors)

    while True:
        nr_chosen_start = int(input("Choose a start index for the shuffle\n"))
        if nr_chosen_start <= nr_max:
            break

    while True:
        nr_chosen_stop = int(input("Choose a stop index for the shuffle\n"))
        if nr_chosen_stop <= nr_max:
            break

    print("Starting point: ", end='')
    print_binary(nr_chosen_start, nr_floors)
    print(" and Finish point: ", end='')
    print_binary(nr_chosen_stop, nr_floors)
    print("\n")

    shuffle(nr_chosen_start, nr_chosen_stop, nr_floors)

    