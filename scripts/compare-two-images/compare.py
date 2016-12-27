# USAGE
# python compare.py
#
# How to Compare Two Images in Python
#   Accessed online: http://www.pyimagesearch.com/2014/09/15/
#                     python-compare-two-images

# import the necessary packages
from skimage.measure import structural_similarity as ssim
import matplotlib.pyplot as plt
import numpy as np
import cv2

def mse(imageA, imageB):
	# the 'Mean Squared Error' between the two images is the
	# sum of the squared difference between the two images;
	# NOTE: the two images must have the same dimension
	err = np.sum((imageA.astype("float") - imageB.astype("float")) ** 2)
	err /= float(imageA.shape[0] * imageA.shape[1])
	
	# return the MSE, the lower the error, the more "similar"
	# the two images are
	return err

def compare_images(imageA, imageB, title):
	# compute the mean squared error and structural similarity
	# index for the images
	m = mse(imageA, imageB)
	s = ssim(imageA, imageB)

	# setup the figure
	fig = plt.figure(title)
	plt.suptitle("MSE: %.2f, SSIM: %.2f" % (m, s))

	# show first image
	ax = fig.add_subplot(1, 2, 1)
	plt.imshow(imageA, cmap = plt.cm.gray)
	plt.axis("off")

	# show the second image
	ax = fig.add_subplot(1, 2, 2)
	plt.imshow(imageB, cmap = plt.cm.gray)
	plt.axis("off")

	# show the images
	plt.show()

# load the images -- the original, the original + contrast,
# and the original + photoshop
inwater = cv2.imread(
    "../../data/reproducible/in-water/iver__28_12_2015_____21_37_38/in_water.png"
    )
sim = cv2.imread(
    "../../data/reproducible/simulation/iver__18_12_2016_____17_26_26/simulation.png"
    )
docker = cv2.imread(
    "../../data/reproducible/docker/repro.png"
    )

# convert the images to grayscale
inwater = cv2.cvtColor(inwater, cv2.COLOR_BGR2GRAY)
sim = cv2.cvtColor(sim, cv2.COLOR_BGR2GRAY)
docker = cv2.cvtColor(docker, cv2.COLOR_BGR2GRAY)

# compare the images
compare_images(sim, docker, "Sim (MacOS) v. Docker (Debian 8)")
compare_images(sim, inwater, "Sim (MacOS) v. In-water (Iver2-167)")
compare_images(docker, inwater, "Docker (Debian 8) v. In-water (Iver2-167)")

