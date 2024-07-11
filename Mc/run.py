from subprocess import Popen, PIPE
import sys

null = file('/dev/null', 'w')

time_cmd = ['time', '-f', '%U %M']

def run1(cmd):
    p = Popen(time_cmd + cmd, shell=False, stdout=null, stderr=PIPE)
    return p.communicate()[1]

for i in range(100):
    print run1(sys.argv[1:]),
