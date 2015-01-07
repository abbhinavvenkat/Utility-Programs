"""Python Script to Download Videos
By Default, it chooses the best format to download the videos"""

print "Welcome to Video Downloader 1.0"
print "Note: By Default, the Best available format is chosen"
print "Also, pafy python package is required to run this program"

import pafy

url = raw_input("Enter URL: ")
video = pafy.new(url)

print "Your Video's Name is: %s"  % video.title
 
best = video.getbest()
print "Downloading from: %s" % best.url

best.download(quiet=False)

print "Thank you for using"
