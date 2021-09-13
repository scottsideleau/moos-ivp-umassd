# Example environment for using MOOS-IvP with flying robots

Assuming a robot can fly by exploiting the concept of negative depth
as a corollary for altitude above sea level (ASL), we observe two 
deficiencies in stock MOOS-IvP infrastructure:

1. BHV_ConstantDepth prohibits negative depths.
2. uSimMarine prohibits negative depths.

As such, we introduce:

1. BHV_SimpleDepth, which permits negative depths.
2. uSimpleRobot, which permits negative depths.

While BHV_SimpleDepth is essentially only a few lines of source code
different from BHV_ConstantDepth, uSimpleRobot is a new application.

An example of how to use the new simulator and behavior are contained
within this mission directory.
