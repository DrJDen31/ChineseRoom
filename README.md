# ChineseRoom
Developed through COGS-4960 Extended Cognition at the Rensselaer Polytechnic Institute during Summer 2023.

Collaborators: Jaden Tompkins, Ngawang Ghongwatsang, Xenia Khusid

Under Professor Bram Van Heuveln

Overview
------
The Chinese Room is a simulation intended for showcasing different perspectives on the Chinese Room thought experiment proposed by John Searle. The thought experiment imagines an enclosed room with a man inside. A sentence in chinese is fed into the room to the man. As he doesn't understand chinese, he consults a rulebook and is able to produce a reply that is fed back out of the room. To an outside observer, the room as a whole seemingly knows chinese, but not a single part of the system actually does. So, does the room understand chinese? This model has been likened to an AI, which takes an input and gives back a seemingly conscious reply, but may not truly be sentient. The simulation puts the player in the position of the man, as they follow rulebooks in order to produce an output. After a variety of situations, the player can get a better understanding of the system and it's level of understanding.

Breakdown
------
The simulation is built in Unreal Engine 5.1 and is mostly made up of extensions of the User Widget class. 
- The player first sees a main menu: BP_MainMenuWidget
- Another submenu allows the player to choose between the various levels unlocked: BP_LevelSelectWidget
- The functionality of the room is through a widget, currently: BP_TheRoomWidgetTest (likely to be renamed)

Each of these derive from a bridge C++ class. The UserWidget class is extended for each one (for example into MainMenuWidget), which is then extended in blueprints (to BP_MainMenuWidget). This allows each to have its own unique C++ logic that it can extend.

Other systems include:
- Save game to save progress
- Layered data structures to store the data for each level/room

All blueprint content is found under the [Content Folder](), broken down into the respective folders.
All C++ content is found under [Source](), with .h files in Public and .cpp files in Private.

ChineseRoomTypes
------
This is a header file containing the data structures used in this project. It is included in any class that may need to use these structures. It starts with the structures used for the data and functionality, then at the end are a few structures for the [Save Game System]()

In order of increasing complexity:
### EShapeSpecialCharacter (SpecialCharacters):
This is an enum containing all of the possible special character the user may be manipulating (as a stand in for chinese). Used for comparisons and to visually represent the level to the user.

### FSpecialCharacterRow (Row):
Due to using TArrays, an extra level of abstraction is needed for 2D arrays, so this struct holds one row of SpecialCharacters.

### FSpecialCharacterImageRow (ImageRow):
Again, due to using TArrays, an extra level of abstraction is needed for 2D arrays, so this struct holds one row of pointers to UImages to reference the grid of images in the blueprint extension to manipulate in C++.

### FWindow (Window):
A Window is a struct that acts as a 2D array of SpecialCharacters. Many things are represented through windows that we will get into later. This allows us to store all these things the same way and easily compare between them.

Note: Currently has an explicit Height and Width variable, but likely to be phased out with Window.Num() being used instead (so these variables don't need to be updated every change).

### FPage (Page/Rule):
A Page is a struct that acts as a left and right page from a RuleBook. The left page contains an *if* and the right page contains a *then*, so a page contains one rule (If you see this, do that). Each half of a Page contains a Window to compare to what you see in the [Workspace](), as well as a text portion for special cases (If you see this, you are done).

### FBook (Book/RuleBook):
A Book is a struct that contains an array of Pages. Books are likely to be themed. For example, one Book may contain all Pages or Rules dealing with circles, and another deals with squares. This gives an experienced user an easier way to remember where certain Rules may be. It also allows certain Rules to direct you to different Books (such as If you see this, do this, then go to Book 2).

### FShelf (Shelf/Bookshelf):
A Shelf is a struct that holds an array of Books. Each Room will have a unique ruleset with the same rules applying to all Levels in that Room, and so a unique Shelf. As the Rooms are inherently different, a Shelf only works for its given Room.

### URoom (Room):
A Room is a simple data UClass. It contains the unique Shelf for that Room as well as a text block that gives the user a description of how this Room may differ from the others (such as "Focus Window is moved automatically").

### UChineseRoomLevel (Level):
A Level is another simple data UClass. A Level is a unique problem, and so it contains a Room to give a specific ruleset, and 3 Windows: the StartingWorkspace, the DesiredOutput, and the FocusWindow. 
- The StartingWorkspace describes the initial conditions of the problem, so the input. The user will look at this and make various changes based on the rules in the ruleset.
- The DesiredOutput describes the condition that, if achieved, means the user has completed this level. It is unknown to the user, but is compared to after each change the user makes.
- The FocusWindow is a small portion of the workspace that the user is currently working on. Due to its implementation, it can likely be **SWITCHED TO INITIAL FOCUS WINDOW POSITION**

### ESolveState (SolveState):
An enum describing the state of the user's progression for a Level.
- Locked: the user can't play this Level and needs to complete the previous ones in the same Room
- NotStarted: the user can play this Level but hasn't yet
- InProgress: the user has played this Level but hasn't completed it and has played another one more recently
- Current: the has played this Level the most recently
- Completed: the user has completed this Level

### FLevelSave (LevelSave):
A struct containing all necessary information for a Level. Has a field for the Blueprint instantiated Level (e.g. BP_LevelX seen [here]()), a field for the SolveState, and an int for the number (e.g. Level 1).

Using Rooms/Levels
------
The simple data classes that make up Rooms and Levels can be extended in Blueprints in order to create the unique instances. For each Level, a BP_LevelX will be made and manually filled out with the specific problem. Similarly, for each Room, a BP_RoomY will be made and each Page of each Book manually filled out. Then, once a Level is selected and a BP_TheRoomWidgetTest is created, the desired Level is fed into the MenuSetup() function to initialize all of the values. The Blueprint instances can be found in [Content/Rooms](Content/Rooms) and [Content/Levels](Content/Levels) respectively.

### Levels vs Rooms:
- A Level is a unique problem, a specific situation: consists of a unique input phrase and a desired output that the user will reach by the end.
- A Room can be thought of as a unique ruleset, a type of situation: consists of a set of RuleBooks and various (currently 5) levels.
- Different levels give the user more experience in each type of situation to get a better feel for it and the chance to solve different problems with the same rules.
- Different rooms give the user a chance to experience different types of situations to take different perspectives on the thought experiment (e.g. does it change your perspective if you are given more or less autonomy in the room and solving different problems with varying complexities).

Main Menu
------
The functionality for the Main Menu is mostly in blueprints, as it mostly just creates and removes widgets. The setup is in C++ through MenuSetup(), which needs to be called right after creating the widget.
### Play / Continue:
Loads a level for the user based on what they played last. Button text is decided and displayed through SetPlayText().
- If it is the first time in the simulation, the text will say "Play" and the first level will be loaded.
- For other cases, the text will read "Continue *LevelNumber*", where *LevelNumber* is the level that will be loaded.
- If the user had been on a level but had not finished it, that level will be loaded.
- If the user had finished the last level they were on, but didn't start another, the next one in chronological order will be loaded.
### Levels:
Creates the BP_LevelSelectWidget allowing the user to select a specific level that they want to play.

### How To Play:
*Currently not implemented:* Will create a widget that gives a basic explanation of how the simulation is played.

### Learn More:
*Currently not implemented:* Will create a widget that gives a basic background on the original Chinese Room thought experiment.

### Settings:
*Currently not implemented:* Will create a new widget that will give a variety of options to the user.

### Quit:
Simply quits the application.

### Refresh:
For testing purposes. Erases all save game data to test if the save game is operating properly.

Level Select
------
This widget displays the levels to the user allowing them to select one to play. When a level is clicked, if unlocked, it will be loaded. 
### Levels vs Rooms:
- In BP_LevelSelect, a level is it's own square with a unique number (e.g. Level 1).
- In BP_LevelSelect, a room is shown by a color coded row (e.g. Levels 1-5 are all blue and part of Room 1).
- More information on the difference between the two can be found [here]().

### Functionality:
Each button will create a BP_TheRoomWidgetTest and add it to the viewport, then calls MenuSetup() and passes in the corresponding level in order to initialize values correctly.

### Progess Icons:
The Save Game System holds an enum that describes the progess of a Level for each Level. So, each Level button has an icon referring to the user's status. These (will) include:
- Check Mark when the Level has been completed
- Clock when the Level has been started but not finished
- Progress Bar when the Level is the most recently played
- None and will be disabled (grayed-out) when the Level has not been unlocked yet (the ones before it in the same Room haven't been completed yet)

### Button Setup:
This function is called on Construct and goes through each Level setting the Progress Icon for it and if it is enabled or disabled.

The Room - UI and BP
------
### Layout


### Help Button
Creates a simple widget, BP_HelpWidget, that displays the help text for the room this level is a part of.

### Quit Button
Returns to the main menu by creating a BP_MainMenuWidget and destroying this widget.

### Testing Button
Will do various things to test new features. Currently simulates the user completing the Level.

### Move Focus Window By
Called from C++, physically moves the BorderFocusWindow, which is the visual representation of the Focus Window by the amount given.

### Set Window Image
Called from C++, given a particular UImage* and EShapeSpecialCharacter, sets the to the desired shape.

### On Level Completed
Called from C++ when the level is completed, simply calls UpdateLevelInfo().

### Update Level Info
Once a Level is completed, we need to do a few things with the save game. We set the bool *bIsSolved* to true, set the SolveState to Completed, and set the next Level in the Room (if there is one) to NotStarted (so it is no longer locked).

### Set Level Number
This function is called when the widget is created, around the same place and time as MenuSetup(). It sets the LevelNumber text in the top right to the current level adn updates which level is the current level in the Save Game System.

The Room - Functionality
------
Most of the functionality for the whole simulation is in this class.
### 


### Move Focus Window
g -- with future plans to implement arrow keys


Save Game System
------

