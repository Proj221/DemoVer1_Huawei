# DemoVer1_Huawei
This is Huawei's part for Porj221 Demo Ver1

## Ep1_Initial Commit.
+ create new project file with UP-DOWN logic.
+ create new git ignore.

## Ep2_Clock Setup.
+ setup global timeline, referenced by https://pastebin.com/U9Re16cY.
+ setup mod calculation based on the wholecycleduration.
+ setup class name and print class properly.

## Ep3_Run Between Target Points.
+ setup behavior tree and blackboard.
+ run behavior tree with choose next target point and move to.
+ setup new AI controller.

## Ep4_Set a GetNextTargetPoint in AI BP
+ add new GetNextTargetPoint in AI blueprint for defining which target point to go. Outdoor / Indoor.
+ setup sub behavior tree to read target point and to set blackboard key.

## Ep5_Get Blackboard Comp and Value in AI BP (with tick)
+ add a new function with tick component to detect blackboard value/key change.
+ remove set blackboard key value in the behavior tree, as it becomes difficult to abort itself in the behavior tree.

## Ep6_Set light change
+ add a new function in LEVEL blueprint (note that this is a dangerous move) for changing the light according to the time (sun degree).

## Ep7_Set camera tweaking
+ add key ALT to enable and disable the camera tweaking
+ add logic to tick flow on checking IsTweakingCamera
+ if is tweaking, then go into tweaking process, containing 1) change the camera spring length and 2) change the camera yaw and pitch.
+ add logic in PlayerController source code to acoomodate the tweaking camera. Only if bMoveToCursor = True and !bTweakingCamera = True, then move to curser.
+ update ignore list to get back Mannequin and other assets for clone convenience.



