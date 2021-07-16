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

## Ep8_Set Decorator in behavior tree
+ add a new decorator in BT to judge the distance between the AI and the player. Note that the owner actor is the AI contoller which needs to be casted into the AI controller and get the controlled pawn.
+ temperarily disconnect the target point to the indoor and outdoor target points.
+ note that the target point is not able to use IsAtLocation to check the validity.

## Ep9_Set another Decorator to BT
+ remove the previous decorator on checking the class on/off
+ class on/off would be sensed in the character, and further to the AIcontroller for defining the next target point (position).
+ add a new decorator to check if the actor is near the target point. Decorator would abort the BT task if the distance is smaller than the thresold set in BT_Deco.
+ TODO: Magic number injected in the decorator. (done)
+ NOTE: Should still check if the decorator can be saved by the distance set by MoveTo.

## Ep10_Add new game state to store game related params
+ add game state to store game preset params and dynamic params
+ refactor chara_body and game mode for accomodating the game state creation
+ TODO: refactor AIController to accomodate the game state creation (Ep11) Partially refactored AIController, need to have a more clear line between the get target vecotr and set blackboard comp key.

## Ep11_Add new target sets
+ add 2 boxes to cover the outdoor space and indoor space
+ generate random vectors when the class on off states are changed
+ move to the randorm generated vectors according to the blackboard key
+ ISSUE01 inject: sometimes the off class status will not trigger AIcontroller to move to outdoor space.

## Ep12_Bug fix
+ ISSUE01 resolve: in level blueprint there are 2 functions duplicate the codes. Two should always be changed together.

## Ep13_Add AI perception
+ add AI sight and hearing perception. Only sight is functional as their is no hearing event trigger.
+ add behavior tree decorator to check the blackboard value on notifiedActor.
+ add a new string set in game state to store the blackboard value.

## Ep14_Add AI control (WASD) and refactor the file struct
+ add AI control with keyboard with WASD (in character)
+ refactor the file struct

## Ep15_Add Spacebar control
+ add spacebar to control the player pawn is sending out the interaction request
+ change logic in behavior tree with 2 pre-requistes before accpeting interaction (1. is notified actor & 2. is receiving interaction request)



