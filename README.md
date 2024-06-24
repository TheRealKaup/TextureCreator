# _TextureCreator_, a tool for creating [KTech](https://github.com/TheRealKaup/KTech) textures.

![TextureCreator Screenshot](https://github.com/TheRealKaup/TheRealKaup/blob/main/texturecreatorscreenshot.png)

## Manual

### Command line arguments

- `texturecreator importfile` - import the given file
- `texturecreator sizeX sizeY` - set the canvas to the given size
- `texturecreator --help` - print this manual

### Keybinds

- Arrow keys to select widget
- Canvas (when it is selected):
    - WASD to move the canvas brush
    - Return (enter) to paint with the brush
    - Backspace to erase foreground, background and character values with the brush
    - Delete to erase according to the toggled tools with the brush
 
### Widgets

- Canvas - the frame in the center of the program, inside it is the texture that can be exported and imported into a KTech game or back into TextureCreator for further edits (note that the size of the exported texture will always be the size of the canvas, as TextureCreator does not attempt to crop any unused outer rows or columns of the canvas when exporting a texture)
- Top section:
    - "Exit" button - exit the program (unexported textures will not be saved)
    - "Import" button - import the texture file "import.ktecht" from the current directory
    - "Export" button - export the texture as a file named "export.ktecht" to the current directory (will override if there is another file named "export.ktecht")
    - "CanvasX" int field - the X size of the canvas, does not apply until the "Confirm" button is pressed
    - "CanvasY" int field - the Y size of the canvas, does not apply until the "Confirm" button is pressed
    - "Confirm" button - applies the values of "CanvasX" and "CanvasY" int fields as the size of the canvas
- Side section:
    - Under "Foreground":
        - "R" int field - the foreground red value of the brush
        - "G" int field - the foreground green value of the brush
        - "B" int field - the foreground blue value of the brush
        - "A" int field - the foreground alpha value of the brush
    - Under "Background":
        - "R" int field - the background red value of the brush
        - "G" int field - the background green value of the brush
        - "B" int field - the background blue value of the brush
        - "A" int field - the background alpha value of the brush
    - "Char" string field - the character value of the brush
    - "BrushX" int field - the X size of the brush
    - "BrushY" int field - the Y size of the brush
    - "Fore Tool" switch - toggle whether the brush should paint foreground color
    - "Back Tool" switch - toggle whether the brush should paint background color
    - "Char Tool" switch - toggle whether the brush should paint characters
    - "Back" int field - the background of the canvas. 0 is black, 1 is red, and 2 is white and gray check pattern
