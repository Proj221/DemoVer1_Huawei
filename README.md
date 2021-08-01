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

## Ep16_Add overhead bubble
+ add new bubble widget with an image
+ add bubble widget to an actor component with root as a cube
+ add child actor component to the character and set the rotation of this child always towards camera
+ add logic in AI perception where when the AI meets the player, the bubble turns red and otherwise remain green

## Ep17_Add PlayerCameraRotation
+ add logic in AIController for checking if a conversation session has been raised
+ add logic in CharaBody for controlling the camera rotation (set and restore) if SpaceBar has been pressed
+ add view control in game state to control the pitch, yaw and roll angle, as well as the camera boom length
+ add logic in CharaBody on the chara body rotation, to make the player faces to the AI once SpaceBar has been pressed

## Ep18_Add new teacher character
+ add new teacher controller
+ add new behavior tree
+ check is my class before going to the class desktop

## Ep19_Add new decorator in studentAI
+ add new decorator @ MoveTo
+ refactor TeacherAI

## Ep20_Add new widget when talking
+ add new widget to HUD when talking with NPC
+ add a button to end conversation sessions
+ add a new check in Character CPP file. If player in conversation, moveToCursor will be disabled.

## Ep21_Resolve keyboard control and mouse control conflicts
+ add a check in playercontroller when moveToCursor
+ add bIsKeyboardControl
+ set bIsKeyboardControl to TRUE if any keyboard input is set.

## Ep22_Add line trace by object to those SM blocking the camera visibility
+ add line trace by objects at each tick
+ add if blocked, then make it transparent
+ bug left: 1st first MoveToCursor, then use keyboard control, the character would not be able to move correctly; 2nd after rotating the camera, WASD would not be able to use correctly.

## Ep23_Fix keyboard control and cursor control conflicts
+ add bCanBreak boolean var in CPP

## Ep24_Fix keyboard direction issue
+ add camera tweak yaw compensation to the movement control
+ add speed control to tweak camera with factor from 1 to 10

## Ep25_Add teacher movement logic
+ MainLevel, add blackboard keys in level blueprint with 2 target points, with 1 as desktop target and the other as the direction
+ MainGameState, change the names in game state
+ CharaBody and CharaTeacher, seperate bodys between teacher and player (student)
+ TeacherAI, set new blackboard keys
+ TeacherAIBlackboard, change teacher logic, add move to and rotate to
+ CharaTeacher, disable use controller rotation yaw, enable use controller desired rotation, to make the rotation smoothly

## Ep26_Seperate Student body with Player Body
+ Add 2 new material instance from M_Male_Body to seperate/distinguish teacher (red) with student (green)
+ Seperate TeacherBody, StudentBody and CharaBody (especially for player)

## Ep27_Change student behavior tree
+ MainLevel, add class name change
+ MainGameState, modify some vars
+ CharaBody, modify movement disable and enable
+ CharaStudent, disable use controller rotation yaw, enable use controller desired rotation, to make the rotation smoothly
+ CharaTeacher, add tags to teacher
+ StudentAI, add some logics to distinguish if the character is in conversation, not in conversation but in already move to, or not yet move to target point
+ StudentAIBlackboard, add new teacher notification
+ StudentAIController, change successfully sensed to student sensed, only sense to student discovery
+ CheckInteractionRaised, interaction has some bugs, how to difference the student with the teacher?
+ ConversationDisplay, minor fix

## Ep28_Refactor Character bodies
+ fix player movement bugs (cannot move correctly when keyboard and curosr control at same time)
+ refactor characters

## Ep29_Make the student look at the teacher
+ make the student look at the teacher once they reach the position

## Ep30_Set correct camera rotation when in conversation
+ pass a new variable from CharaStudent to CharaBody with LookAtLocation
+ compensate the camera position with new LookAtLocation
+ move disable and enable movement to a better position

## Ep31_Add a debug key on game speed
+ press 0 to accel 5 times faster than the current game speed

## Ep32_Set new student behavior
+ seperate bubble comp with mood and emoji. Emoji would reflect by notifying the actors while mood would reflect by the sanity
+ add new behavior branches

## Ep33_Set sanity value
+ add sanity concept to student AI
+ add numerical display on top of the bubble comp
+ set sanity drop rate with 4 levels, student would loss level1 + level2 when they are listening, but only level1 when they are idling
+ refactor

## Ep34_Add sanity zero BB
+ add sanity zero check in BB for seperate sanity high, low with zero
+ when sanity drops to zero, AI would increase sanity value, assuming they are in chating with others

## Ep35_Change peripheral vision
+ misunderstand the peripheral vision, as the angle indicate here stands for the vision angle for ONE eye
+ change notified actor logic to only detect player
+ change emoji content to only change when detecting the player (will not change to other player)


