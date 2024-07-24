# Trivia-Board-Game

## Description

This is a simple trivia game coded using C programming that can be run through the terminal. The application features a game phase and admin phase and allows the user to customize their game.

## Installation

1. Download the 5 files
2. Compile and run the main.c file in the terminal or in an IDE

## Usage

In the <b>game phase</b>, the user is first asked to import a text file containing trivia questions. A sample text file, sample.txt, has been provided. Then, the user sets the dimensions of the board game and the board is initialized so that no single row contains two trivias starting with the same letter and case. The game then starts, and the user must successfully guess at least one trivia from each row to win the game.

<div class="row" display="flex" justify-content="center">
  <img alt="Game Menu" src="https://github.com/KirstenTan/Trivia-Board-Game/blob/main/images/Game%20Menu.png" width="20%">
  <img alt="Play Game" src="https://github.com/KirstenTan/Trivia-Board-Game/blob/main/images/Play%20Game.png" width="20%">
</div>

In the <b>admin phase</b>, the user is presented with a menu containing the following options: Add Word, Add Trivia, Modify Entry, Delete Word, Delete Clue, View Words, View Clues, Export, and Import. This allows the user to modify the list of trivia included in the game, save the current list of trivia, and load a previously saved list of trivia.

<div class="row">
  <img alt="Admin Menu" src="https://github.com/KirstenTan/Trivia-Board-Game/blob/main/images/Admin%20Menu.png" width="20%" display="flex">
  <img alt="Add Word" src="https://github.com/KirstenTan/Trivia-Board-Game/blob/main/images/Add%20Word.png" width="20%" display="flex">
</div>
