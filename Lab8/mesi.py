import sys
from tabulate import tabulate

no_of_proc = 3
proc_states = {}
cache_no = -1
has_data = 0
data = []
headers = []

'''
 X <- BusRdX
 S <- BusRd(S)
 # <- BusRd(S#)
 - <- BusRd/- or just -
 F <- BusRd/Flush*
'''

# Test function - runs the test given as an example in the lab
def test():
    params = [0, "r", 1, "r", 2, "r", 0, "r", 0, "w", 0, "r", 1, "w", 2, "r"]
    global no_of_proc
    no_of_proc = 3
    init_headers()
    init_states()
    for i in range(0, len(params), 2):
        proc_id = int(params[i])
        proc_action = params[i + 1]
        run(proc_id, proc_action, i)
    print(tabulate(data, headers=headers, tablefmt="grid"))

# Initializes the headers for the table
def init_headers():
    headers.append('T')
    headers.append('Actiune Procesor')
    for i in range(no_of_proc):
        headers.append('Stare' + str(i + 1))
    headers.append('Actiune Magistrala')
    headers.append('Sursa date')
    
# Appends the initial state of the automaton to the data list
def init_states():
    entry = []
    entry.append('T0')
    entry.append('init')
    for i in range(no_of_proc):
        proc_states[i] = 'I'
        entry.append(proc_states[i])
    entry.append('-')
    entry.append('-')
    data.append(entry)
    
# Runs the automaton for each step of the process
def run(proc_id, proc_action, i):
    action = ''

    action = active_proc_decision(proc_id, proc_action)
    change_active_proc(proc_id, proc_action)
    change_passive_procs(proc_id, proc_action)

    entry = []

    entry.append('T' + str(int((i + 2) / 2)))
    entry.append('P' + str(proc_id + 1) + ('Rd' if (proc_action == 'r') else 'Wr'))

    for proc in range(no_of_proc):
        entry.append(proc_states[proc])

    entry.append(parse_action(action))
    entry.append(parse_source(action))

    data.append(entry)

# Decides the action of the active proc based on it's state and the chosen action
def active_proc_decision(proc_id, proc_action):
    if proc_states[proc_id] == 'I':
        if proc_action == 'w':
            return 'X'
        else:
            for state in proc_states:
                if proc_states[state]  == 'S' or proc_states[state] == 'E' or proc_states[state] == 'M':
                    return 'S'
            return '#'

    if proc_states[proc_id] == 'S':
        if proc_action == 'w':
            return 'X'
        else:
            if proc_states[proc_id] == 'S':
                return '-'
            for state in proc_states:
                if proc_states[state]  == 'S' or proc_states[state] == 'E' or proc_states[state] == 'M':
                    return 'F'
                
    if proc_states[proc_id] == 'E':
        return '-'

    if proc_states[proc_id] == 'M':
        return '-'

    return 'INVALID'

# Modifies the state of the active proc based on the chosen action
def change_active_proc(proc_id, proc_action):
    global cache_no
    if proc_states[proc_id] == 'I':
        if proc_action == 'w':
            proc_states[proc_id] = 'M'
            return
        else:
            for state in proc_states:
                if proc_states[state]  == 'S' or proc_states[state] == 'E' or proc_states[state] == 'M':
                    proc_states[proc_id] = 'S'
                    cache_no = state
                    return
            proc_states[proc_id] = 'E'
            return

    if proc_states[proc_id] == 'S':
        if proc_action == 'w':
            proc_states[proc_id] = 'M'
        return
                
    if proc_states[proc_id] == 'E':
        return

    if proc_states[proc_id] == 'M':
        return

# Modifies the state of the passive procs based on the chosen action
def change_passive_procs(proc_id, proc_action):
    for i in range(len(proc_states)):
        if i == proc_id:
            continue
        if proc_states[i] == 'M':
            proc_states[i] = 'I' if proc_action == 'w' else 'S'
        if proc_states[i] == 'E':
            proc_states[i] = 'I' if proc_action == 'w' else 'S'
        if proc_states[i] == 'S':
            proc_states[i] = 'I' if proc_action == 'w' else 'S'
        if proc_states[i] == 'I':
            proc_states[i] = 'I' if proc_action == 'w' else 'I'

#Decodes the source from the chosen action of the automaton
def parse_source(character):
    if character == '#':
        return 'Mem'
    if character == 'S':
        return 'Cache' + str(cache_no + 1)
    if character == '-':
        return '-'
    if character == 'X':
        return 'Flush'
    
# Decodes the action from the chosen action of the automaton
def parse_action(character):
    global has_data
    if character == '#':
        has_data = 1
        return 'BusRd'
    if character == 'X':
        has_data = 0
        return 'BusRdX'
    if character == '-':
        return '-     '
    if character == 'S':
        return 'Flush' if has_data == 1 else 'BusRd'

# Prints the formatted output and calls the output for each step
if __name__ == "__main__":
    print('Usage (index starts at 0): ($ python3 mesi.py test) | ($ python3 mesi.py no_of_proc 0 r/w 1 r/w ... (no_of_proc - 1) r/w)\n')

    if sys.argv[1] == 'test':
        test()
        exit()

    if (len(sys.argv) - 1) % 2 == 0 or len(sys.argv) == 1:
        exit()

    no_of_proc = int(sys.argv[1])
    init_headers()
    init_states()

    for i in range(2, len(sys.argv), 2):
        proc_id = int(sys.argv[i])
        proc_action = sys.argv[i + 1]
        run(proc_id, proc_action, i-1)

    print(tabulate(data, headers=headers, tablefmt="grid"))

