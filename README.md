# Title

Todo is a simple terminal application for keeping todo lists.

## To connect with your Google Drive

IMPORTANT NOTE

Requires that you have already authed and have a refresh token and client id and client secret

you can use these instructions to get authed...

### Enabling Google OAuth for an API (Drive)
    1. On google cloud developers console created a new project
    2. On google cloud developers console enabled API i wanted (drive)
    3. On gcdc created credentials
        - OAtuh client creds
        - Desktop App
        - copied Id and Secret safely
    4. fill in and run this command:
        client_id="..."
        scope="https://www.googleapis.com/auth/drive"
        url="https://accounts.google.com/o/oauth2/auth?client_id=$client_id&redirect_uri=urn:ietf:wg:oauth:2.0:oob&scope=$scope&response_type=code"
        echo "$url"
    5. copy and paste said url into browser
    6. authorise, if problems because app is not verified or published, step up a test user
        - go to OAuth consent screen and add your gmail account as a test user
    7. authorise and copy down the code that the browser gives after authorising! [code]
    8. fill in and run this command: (curling)
        code="..." (from previous screen)
        client_secret="...."
        curl --request POST --data "code=$code&client_id=$client_id&client_secret=$client_secret&redirect_uri=urn:ietf:wg:oauth:2.0:oob&grant_type=authorization_code" https://accounts.google.com/o/oauth2/token
    9. from this response take note of the token and refresh token

----
what else can i say about it?

## Sub tutle

----

### Now I want to show you

That this is great

# Todo List 

DONE - >* Order print out of todo table by list order not by ticket order
* Help -h arguement output correctly
* Create a -m flag (mass edit) pass in numbers seperated by commas
e.g 1,5,66,3...  you can then change their tag, pri, and list in a mass edit but NOT the title
DONE - >* Edit a single title gives you it in editor
* Add notes to each todo that can be displayed or not
* Handle a syncing function to get todo lists anywhere! - use git?
* Order list by priority or by number taking them out of their lists
* Be able to add prirorities couldnt add a new one
DONE - >* Deleted items go to the backup file instead
* Add Man page 
* Add timestamps to each todo
* Add a todo log in the ~/.todo/ folders to log todo transactions ,add, delete, move etc...
* Filter by tag - Github issue #1
* Installaiton script or setup function to build folder and files etc...
