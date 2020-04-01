#!/usr/bin/python

#
#  XcodeVersionCheck.py
#

import re, sys, os, subprocess


expectedMajor = 7
expectedMinor = 1

def run_process(cmd, ignore_errors=False):
	#print "\n*** starting command ***\n" + cmd
	myprocess = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
	out = myprocess.stdout.read().strip()
	#print out

	if (myprocess.wait() != 0) and not ignore_errors:
		#print "*** got a nonzero return code from the shell, dying now. ***"
		#traceback.print_stack()
		#failure(out + "\n" + cmd)
		return ""
	else:
		return out


out = run_process("xcodebuild -version")
#print out

# Check results of 'xcodebuild -version' against expected xcode verion:
mo = re.match(r'\D*(\d+)\.(\d+)', out, re.I)
if mo and mo.lastindex > 1:
	#print "lastindex = %d" % mo.lastindex
	major = int(mo.group(1))
	minor = int(mo.group(2))
	print "%d.%d" % (major, minor)
	if (major == expectedMajor and minor >= expectedMinor)  or  (major > expectedMajor):
		sys.exit(0)		# Xcode version is good

# If we didn't exit above, then something is wrong with the Xcode version
print "You must have Xcode version %d.%d or later installed and configured for use" % (expectedMajor, expectedMinor)
sys.exit(1)