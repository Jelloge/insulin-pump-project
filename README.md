## COMP3004 Team Project
## Tandem t:slim x2 Diabetes Insulin Pump

Group Number: 50

Group Members:
Aurelie Mei Yin Ng Yin Hing (101246374)
Nikhil Bansal (100997597)
Laven Joshi (101272355)
Jasmine (101156769)

### Design Explanation:

A new CGM reading is taken every 10 seconds in real time. New readings are shown on the CGM Graph on the home screen of the t:slim. 

Observer Design Patterns were used in this project by connecting Signals and Slots.

What each team member did:

### Nikhil:
- Options Menu:
    - Stop/Start Insulin
    - Load:
        - Change Cartridge
        - Fill Tubing
        - Fill Cannula
        - Site Reminder
    - Activity:
        - Temporary Basal Rate
    - My Pump:
        - Personal Profiles
        - Control-IQ
        - Alerts and Reminders
        - Pump Info
    - My CGM:
        - Start/Stop Sensor
        - Calibrate CGM
        - Transmitter ID
        - CGM Info
    - Device Settings:
        - Display Settings
        - Bluetooth Settings
        - Time and Date
        - Sound Volume

### Jasmine:
- Main GUI Home screen
- Battery Functionality
- Clock Functionality
- Power functionality
- History Settings:
	- Bolus, CGM, Alerts History
- Device Settings:
	- Security PIN

### Aurelie:
- Glucose Monitoring
    - Graphs/Simulated Readings
    - Glucose level alerts
    - Glucose History

### Laven:
- Bolus Calculator
- Enable access to full insulin history
- Store last insulin injection (extended or immediate)


## Organization of the Submission ##

Files Submitted:

### - Use Case Model.pdf
### - Requirements Traceability Matrix.pdf
### - readME.md
	
### - UML Diagrams
    - Use Case Diagram.png
    - UML Class Diagram.png
    
### - Sequence Diagrams
    - Main Success Scenario.png
    - Safety Scenario 1 Cancel Bolus.png
    - Safety Scenario 2 Forgotten Pin.png
    
### - State Diagrams
    - Insulin Pump Controller State Machine.png
    - Personal Profiles State Machine.png

### - Video Recordings - Correspond to their respective Sequence Diagrams
    - Main_Success_Scenario.mp4
    - Safety_Scenario_1.mp4
    - Safety_Scenario_2.mp4
### - /src
        - BolusInfo.h
        - COMP3004Project.pro
        - batterymanager.cpp
        - batterymanager.h
        - boluscalculator.cpp
        - boluscalculator.h
        - bolusmenu.cpp
        - bolusmenu.h
        - bolusmenu.ui
        - config.cpp
        - config.h
        - glucosemonitoring.cpp
        - glucosemonitoring.h
        - historylogger.cpp
        - historylogger.h
        - HistoryLogger.ui
        - main.cpp
        - mainwindow.cpp
        - mainwindow.h
        - mainwindow.ui
        - optionsmenu.cpp
        - optionsmenu.h
        - optionsmenu.ui
        - personalprofiles.cpp
        - personalprofiles.h
        - personalprofiles.ui
        - profile.h
        - timeProfile.h
        - timedSettingsUtils.cpp
        - timedSettingsUtils.h
        - timedsettingsprofiles.cpp
        - timedsettingsprofiles.h
        - timedsettingsprofiles.ui
        - ui_icons.qrc
        - /ui_icons
            - back.png
            - charging.jpg
            - check.png
            - close.png
            - graydiamond.png
            - letter-b.png
            - plus.png
            - resources.qrc
    - .gitignore

## Running Instructions:

	### - Download and extract the NAME_team50_comp3004_groupproject.zip file in the VM
	### - Open QT Creator
	### - File > Open File or Project > Open InsulinPumpProject > Open COMP3004Project.pro
	### - Build the project
### - Run the project
    	### - Press the "ON" button
    	### - Enter the security PIN 3004
    	### - Select "Options" > "My Pump" > "Personal Profiles"
    	### - Create a new profile
    	### - Hit Tandem logo to return to home page
    	### - Select "Bolus"
    	### - Enter in Carbohydrates value and Current Blood Glucose (or use auto-populated value)
    	### - Enter Deliver Now and Deliver Later values if extended bolus selected
    	### - Press Calculate Bolus to see amount of insulin to be injected
	### - User may navigate to “Options” > “History” to view all prior insulin administrations, as well as all past CGM readings and alerts.
    	### - User can toggle between 1, 3, 6, 12 and 24 hours of CGM Readings
    	### - User can navigate through Options Menu. Most of the submenus are functional.




