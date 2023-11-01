import math

# Converts an integer to a binary string in big endian
def integer_to_binary(number):
    binary_str = bin(number)[2:]
    return binary_str

def shuffle(input, size):
    return input // 2 if input % 2 == 0 else (1 << (size - 1)) | (input // 2)

# Simulate the network
def network(start_int, stop_int, len, nr_floors):
    start = integer_to_binary(start_int)
    stop = integer_to_binary(stop_int)
    output = ['' for _ in range(nr_floors)]
    output_step = 0
    current_floor = 1
    network_size = len
    floor_start = [[0, 0] for _ in range(100)]

    # In the first half, all connections can be direct, and the value at each step is saved.
    # If the step is in a network larger than 2, the result is halved.
    for output_step in range(nr_floors // 2 + 1):
        output[output_step] = 'd'
        start_int = shuffle(start_int, network_size)
        floor_start[output_step][0] = start_int
        if start_int >= (1 << (len - output_step)) // 2 and output_step < len // 2 and nr_floors > 3:
            start_int -= (1 << (len - output_step)) // 2
            floor_start[output_step][1] = 1
        else:
            floor_start[output_step][1] = 0
        network_size -= 1

    # In the middle, connection is decided by the MSB
    if start[0] == stop[0]:
        output[output_step] = 'd'
    else:
        output[output_step] = 'i'
        start_int ^= 1
    output_step += 1
    floor_start[output_step][0] = start_int
    floor_start[output_step][1] = 0
    network_size = len

    # Begin from the opposite direction starting with the stop point and move left.
    # At each step, attempt to reach the same half of that level in a greedy manner.
    # Halve the result if necessary.
    for i in range(nr_floors - 1, output_step, -1):
        if stop_int & 1:
            if floor_start[nr_floors - i - 1][1]:
                output[i] = 'd'
                stop_int = shuffle(stop_int, network_size)
            else:
                output[i] = 'i'
                stop_int = shuffle(stop_int ^ 1, network_size)
        else:
            if floor_start[nr_floors - i - 1][1]:
                output[i] = 'i'
                stop_int = shuffle(stop_int ^ 1, network_size)
            else:
                output[i] = 'd'
                stop_int = shuffle(stop_int, network_size)
        if stop_int >= (1 << len) // 2:
            stop_int -= (1 << len) // 2
        network_size -= 1

    # The loop above stops at the network size of 4x4. If numbers are equal, proceed.
    # Otherwise, check number parity and adjust accordingly for matching results.
    # After successful loop, numbers should differ by only one bit (parity).
    if stop_int == start_int:
        output[output_step] = 'd'
    else:
        if stop_int > start_int:
            if stop_int & 1:
                output[output_step] = 'i'
                stop_int = shuffle(stop_int ^ 1, network_size)
            else:
                output[output_step] = 'd'
                stop_int = shuffle(stop_int, network_size)
        else:
            if stop_int & 1:
                output[output_step] = 'd'
                stop_int = shuffle(stop_int, network_size)
            else:
                output[output_step] = 'i'
                stop_int = shuffle(stop_int ^ 1, network_size)

    # Print the result
    for o in output:
        if o == 'd':
            print("direct ", end="")
        else:
            print("invers ", end="")
    print()

# Returns the number of figures
def number_of_figures(number):
    figures = 0 if number == 0 else 1
    while number != 0:
        number //= 10
        figures += 1
    return figures

# Prints number in binary in fixed form
def print_binary(number, length):
    binary_str = integer_to_binary(number)
    print(binary_str.zfill(length), end="")

# Prints table with all possible connections
def print_map(size, log_size):
    print("Index  Start  Stop")
    for i in range(size + 1):
        print(f"{i:2d}      ", end="")
        nr_figs = number_of_figures(i)
        print_binary(i, log_size)
        print("   ", end="")
        print_binary(i, log_size)
        print()

if __name__ == "__main__":
    nr_max = 0

    while nr_max <= 0 or nr_max % 2 == 1:
        nr_max = int(input("Enter network maximum dimension (max input): "))

    max_pow = int(math.log2(nr_max))
    nr_floors = 2 * max_pow - 1
    nr_max -= 1

    print(f"The chosen network has {nr_floors} floors and accepts values between 0 and {nr_max}")

    print_map(nr_max, max_pow)

    list_size = 0
    while list_size == 0:
        list_size = int(input("Choose number of pairs to enter: "))

    nr_chosen_start = []
    nr_chosen_stop = []

    print("Enter pairs of input and output like: \nin1\nout1\nin2\nout2...")
    for _ in range(list_size):
        start = int(input())
        stop = int(input())
        nr_chosen_start.append(start)
        nr_chosen_stop.append(stop)

    for i in range(list_size):
        print(f"Starting point: ", end="")
        print_binary(nr_chosen_start[i], max_pow)
        print(f" and Finish point: ", end="")
        print_binary(nr_chosen_stop[i], max_pow)
        print()

        # Input processing done, starting shuffle
        network(nr_chosen_start[i], nr_chosen_stop[i], max_pow, nr_floors)
