from os import listdir
from os.path import isfile, join, splitext
import time, subprocess, shlex

IN_FOLDER  = "./tests/in"
OUT_FOLDER = "./tests/out"
REFS       = "./tests/ref"
LOGS       = "./tests/logs"

SCORE = 0
serial_times = {}
run_times    = {}

def execute_command(command, timeout=None):
    cmd = shlex.split(command)
    subprocess.check_call(cmd, timeout=timeout)

def setup():
    try:
        subprocess.check_call("make")
        return True
    except:
        print("Homework does not compile")
        return False
    finally:
        print()

def get_files(folder):
    files = [f for f in listdir(folder) if isfile(join(folder, f))]
    files.sort()

    return files

def get_serial_times():
    print("~~~~~~~~~~~~~~~ ESTABLISHING BASE TIME ~~~~~~~~~~~~~~")
    files = get_files(IN_FOLDER)

    for f in files:
        start_time = time.time()
        execute_command("./serial " + IN_FOLDER + "/" + f)
        serial_times[f] = time.time() - start_time
        print("Test " + f + " took " + str(serial_times[f]) + " seconds")
        execute_command("mv ./result.out ./tests/ref/" + splitext(f)[0] + ".ref" )
        
    print()

def run_tests():
    print("~~~~~~~~~~~~~~~~~~~ RUNNING TESTS ~~~~~~~~~~~~~~~~~~~")
    files = get_files(IN_FOLDER)

    for f in files:
        start_time = time.time()
        timeout = max(3, 4 * serial_times[f])
        command = "mpirun -oversubscribe -np 5 ./main " + IN_FOLDER + "/" + f
        try:
            execute_command(command, timeout=timeout)
            run_times[f] = time.time() - start_time
            print("Test " + f + " took " + str(run_times[f]) + " seconds")
            execute_command("mv " + IN_FOLDER + "/" + splitext(f)[0] + ".out " + OUT_FOLDER)
        except subprocess.TimeoutExpired:
            print("Test " + f + " took too long")
        except:
            print("Test " + f + " did not finish")
        
    print()

def check_if_multiple_read():
    files = get_files(IN_FOLDER)
    command = "mpirun -oversubscribe -np 5 ./main " + IN_FOLDER + "/" + files[-1] + \
                " & sleep 0.5 && lsof tests/in/input5.txt | wc -l"

    p = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    output, _ = p.communicate()

    execute_command("rm " + IN_FOLDER + "/" + splitext(files[-1])[0] + ".out ")
    
    if int(output) >= 5:
        return True

    return False

def compare_files(f1, f2, name="log"):
    try:
        command = "diff -B -Z " + f1 + " " + f2
        log = open(LOGS + "/" + name + ".log", "w")
        subprocess.check_call(shlex.split(command), stdout=log, stderr=log)
        log.close()
        return True
    except:
        return False

def compute_score():
    global SCORE

    print("~~~~~~~~~~~~~~~~~~~~~ COMPARING ~~~~~~~~~~~~~~~~~~~~~")
    outs = get_files(OUT_FOLDER)
    refs = get_files(REFS)
    
    for r in refs:
        if (splitext(r)[0] + ".out") in outs:
            if compare_files(OUT_FOLDER + "/" + splitext(r)[0] + ".out", REFS + "/" + r, splitext(r)[0]):
                    SCORE += 2
                    print("Test " + splitext(r)[0] + " is correct\t" + "SCORE = " + str(SCORE))
            else:
                print("Test " + splitext(r)[0] + " failed. Check log.\t" + "SCORE = " + str(SCORE))
        else:
            print("Test " + splitext(r)[0] + " has no result\t" + "SCORE = " + str(SCORE))

    print("\n~~~~~~~~~~~~~ DOES IT READ IN PARALLEL? ~~~~~~~~~~~~~")
    if not check_if_multiple_read():
        SCORE -= 2
        SCORE = max(SCORE, 0)
        print("IT DOES NOT")
    else:
        print("IT DOES")
    print("SCORE = " + str(SCORE))

    print("\n~~~~~~~~~~~~~~~~~~~ DOES IT SCALE? ~~~~~~~~~~~~~~~~~~")
    scalability = True
    for k in serial_times:
        if k not in run_times:
            scalability = False
            print("Missing output for test " + k)
            break

        i, j = serial_times[k], run_times[k]
        if i > 3 and i < j:
            print("Serial : " + str(i) + " vs Distributed : " + str(j))
            scalability = False
            break

    if scalability:
        print("IT DOES :)")
    else:
        print("IT DOES NOT :(")
        SCORE -= 2
        SCORE = max(SCORE, 0)
    
    print("SCORE = " + str(SCORE))

def cleanup():
    files = get_files(OUT_FOLDER)
    for f in files:
        execute_command("rm " + OUT_FOLDER + "/" + f)
    
    files = get_files(REFS)
    for f in files:
        execute_command("rm " + REFS + "/" + f)
    
if __name__ == "__main__":    
    if not setup():
        exit()
    
    get_serial_times()
    run_tests()
    compute_score()
    cleanup()

    print("\nFinal grade = ", SCORE)
