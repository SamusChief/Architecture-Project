COMPILER='Compiler'
INCLUDE='code.h'
SIMULATOR='Multi-Cycle-Simulator.c'

make $COMPILER   ## Ensure your compiler is up to date
shopt -s nullglob
for f in *.asm ## all asm files in directory
do
  robot=$(basename $f)
  if [[ $robot == *"grade"* ]]  ## we don't want the grade asm files, they don't do anything
  then
    continue
  fi
  robot="${robot%.*}" ## grab file extension
  echo $robot ## Lets print our robot name so we know which ones are running
  ./$COMPILER $robot".asm" $INCLUDE  ## compile asm code into include
  g++ $SIMULATOR -o $robot
  if ! grep -q $robot" "$robot "robots.txt"
  then
     echo $robot" "$robot >> robots.txt
  fi
done
~olano/robotarena/RobotDriver -w 8080
