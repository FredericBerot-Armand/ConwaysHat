# ConwaysHat

A jolie "frenchy or not" flashmob choregraphy simulator!

All participants will be arranged as a grid (8 neighbours around).
Each participant carries a set of colored hats ordered in a list (same for everyone), and wears only one. Before the event starts, they choose randomly a hat to wear.

A rooster is used to orchestrate the choregraphy.
Uppon a the rooster screams, participants will look around them and simultaneously (not one at a time) apply the following rule:


Each participant counts the number of neighbours who wears their next colored hat in the list. If this number is greater than the threshold, then he/she will wear its next colored hat. Else the participant doesn't switch hat.

A jolie frenchy example :

![](https://github.com/FredericBerot-Armand/ConwaysHat/blob/main/ConwayHat.gif)

## Required tools

To run ConwaysHat, you will need :

 - [Make](https://www.gnu.org/software/make/)
 - [gcc](https://gcc.gnu.org/)

## Build

Run make to build :
```bash
make
```

## Running the app

```bash
./conways-hat.exe [-t THRESHOLD] [-c COLOR [COLOR]...] WIDTH HEIGHT DELAY
```
with
* COLOR A value among black, red, green, yellow, blue, purple, cyan and white (Participants changes their hat's colors according to the list order) (default : red white blue)
* THRESHOLD The neigthbours' threshold to change the hat's color of a participant (default : 3)
* WIDTH The width of the grid
* HEIGHT The height of the grid
* DELAY The delay between 2 rooster screams (int millisecond)

Exemple :
```bash
./conways-hat.exe 50 50 20
./conways-hat.exe -t 4 100 50 50
./conways-hat.exe -c red blue green yellow purple cyan white -l 2 100 50 50
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)
