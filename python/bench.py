import argparse
from subprocess import Popen, PIPE
import time

import keyboard

FA2_START_FLAG = 'Starting simulation'
BENCH_RESULTS = 'Delta run time'


def read_input(input_name):
    """
    Reads names of graph files from input file
    :param input_name: name of input file
    :return: list of names of graph files
    """
    graph_files = []
    with open(input_name, 'r') as input_file:
        for line in input_file.readlines():
            graph_files.append(line.replace('\n', '').strip())
    return graph_files


def press(key, interval=0.5, after=1):
    """
    Simulates pressing a key on the keyboard
    :param key: key to be pressed
    :param interval: interval between pressing and releasing the key
    :param after: delay after releasing the key
    :return:
    """
    keyboard.press(key)
    time.sleep(interval)
    keyboard.release(key)
    time.sleep(after)


def get_result(stdout):
    """
    Gets benchmark results from stdout
    :param stdout: stdout of process
    :return: benchmark results
    """
    for line in iter(stdout.readline, ''):
        line = line.decode('utf-8')
        if BENCH_RESULTS in line:
            print(line.replace('\n', ''), flush=True)
            return line.replace('\n', '').strip().split(' ')[-1]
    return 0


def write_results(results, input_name, commit):
    """
    Writes benchmark results to file
    :param results: results dict
    :param input_name: name of input file
    :param commit: hash of ForceAtlas2 commit
    :return:
    """
    output_name = input_name.split('.')
    output_name[0] = '{}-{}'.format(output_name[0], commit)
    output_name[-1] = 'csv'
    output_name = '.'.join(output_name)

    with open(output_name, 'w') as output_file:
        output_file.write('# Commit {}\n'.format(commit))
        output_file.write('Graph File,Delta Run Time\n')
        for key, value in results.items():
            output_file.write('{},{}\n'.format(key, value))


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', '--input', required=True)
    parser.add_argument('-c', '--commit', required=True)
    args = vars(parser.parse_args())

    graph_files = read_input(args['input'])
    results = {}

    for graph_file in graph_files:
        print('Running ForceAtlas2Sim with {}'.format(graph_file), flush=True)
        cmd = 'Debug\\forceatlas2sim.exe -i test\\{} -debug'.format(graph_file)
        p = Popen(cmd, shell=True, stdout=PIPE, stderr=PIPE)

        print('Waiting for simulation to start', flush=True)
        for line in iter(p.stdout.readline, ''):
            if FA2_START_FLAG in str(line) or p.poll() is not None:
                break

        # Runs simulation for 30 seconds, then closes it
        press('p', after=30)
        press('p')
        press('esc')

        # Closes simulation if still running
        if p.poll() is None:
            p.terminate()

        # Gets benchmark results for current graph file
        results[graph_file] = get_result(p.stdout)

    # Writes results of all benchmarks to file
    write_results(results, args['input'], args['commit'])
