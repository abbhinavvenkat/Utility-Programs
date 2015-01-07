"""Python Script to Download Videos
By Default, it chooses the best format to download the videos"""

print "Welcome to Video Downloader 1.0"
print "Note: By Default, the Best available format is chosen"
print "Also, pafy python package is required to run this program"

import pafy
import os
import sys

url = raw_input("Enter URL: ")
video = pafy.new(url)

print " "
print "Your Video's Name is: %s"  % video.title

path = ""
path = raw_input("Enter your Path for Saving the File(Press Enter for Current Directory): ")

best = video.getbest()

if path != "":
    if os.path.isdir(path)==False:
	sys.exit("Invalid Path")
    else:
	print ""
	print "Downloading from: %s" % best.url
        best.download(filepath = path)
else:
    best.download(quiet=False)

print " "
print "Thank you for using"
