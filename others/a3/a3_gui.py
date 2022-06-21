from tkinter import *
from tkinter.filedialog import askopenfile

import a3

"""
v 1.0
Patrick M. Dennis, MD
May 10, 2017
GUI for use with module a3.py, assignment 3 of
Coursera's "Learn to Program: The Fundamentals by University of Toronto."
(But not endorsed by anyone other than myself.)
"""
class WordGui():
    def __init__(self, parent):
        '''
        Set up parent window and frame containing text box, an input widget, a list box and labels
        '''
        self.root = parent
        self.root.title("Word Search")

        #instance variables
        self.game_end = False
        self.players = []
        self.found_words_list = []
        self.player_num = 0
        self.invalid_word = False
        #string variables
        self.trial_word = StringVar()
        self.found_words = StringVar()
        self.num_words_rem = StringVar()
        self.player_name_message = StringVar()
        self.scores_message = StringVar()
        #widgets
        self.main_frame = Frame(self.root)
        self.text_frame = Frame(self.main_frame, borderwidth=5, relief="sunken", width=200, height=200)
        self.text_box = Text(self.text_frame, height = 12, width = 30)
        self.word_entry = Entry(self.main_frame, width=7)
        self.scores_lbl = Label(self.main_frame, textvariable = self.scores_message)
        self.word_entry_lbl = Label(self.main_frame, textvariable = self.player_name_message)
        self.num_words_remaining = Label(self.main_frame, textvariable = self.num_words_rem)
        self.lst_box = Listbox(self.main_frame, listvariable=self.found_words)
        #grid positions
        self.main_frame.grid(column=0, row=0)
        self.text_frame.grid(column=0, row=1, columnspan=3)
        self.word_entry.grid(column=1, row=3, columnspan=2,sticky=(W, E))
        self.scores_lbl.grid(column=0, row=0, columnspan=5)
        self.word_entry_lbl.grid(column=0, row=3)
        self.num_words_remaining.grid(column=4, row=3, columnspan=2, sticky=(W, E))
        self.lst_box.grid(column=4, row=1, rowspan=2, sticky=(N,S,E,W))
        self.text_box.grid(column = 0, row = 1, columnspan = 3)

        #resizing
        # TODO: resize text frame; add scrolling of found words list
        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(0, weight=1)

        # actions
        self.text_box['state'] = 'disabled'
        self.word_entry.config(textvariable = self.trial_word)
        self.word_entry.bind('<Key-Return>', self.word_entered)

        self.start_game()

        # found_words textcontrol
    def set_found_words(self, found_words_list):
        '''
        take the found words list and send it to the StrVar()  for the listbox
        '''
        self.found_words.set(found_words_list)

        #num_words_rem text control
    def set_num_words_rem(self, num):
        '''
        (int) -> str
         take num, number of remaining words and
         compose it in a string message to send to StrVar() for label
        '''
        message = 'words remaining: ' + str(num)
        self.num_words_rem.set(message)

        # player_name label text control
    def set_player_name(self, num, message = ', enter a word.'):
        '''
        Take num, index of next player, put it in StrVar() to send to label,
        to display next player up.  If game over, message displays options
        '''
        if not self.game_end:
            player_name = self.players[num][0]
            message = player_name + message
            self.player_name_message.set(message)
        else:
            self.player_name_message.set(message)

        # set trial words entry box to blank
    def reset_trial_word(self):
        '''
        set text in trial_word input to ''
        '''
        self.trial_word.set('')

        # update scores label text
    def set_scores(self):
        '''
        update StrVar() displaying scores, or winner
        '''
        message = ""
        if not (self.game_end or self.invalid_word):
            message = ''
            for player in  self.players:
                message = message + player[0] + ' ' + str(player[1]) + ' * '
            message = "Scores:   " + message
        elif self.game_end:
            message = self.winner + " wins!! "
        elif self.invalid_word:
            self.invalid_word = False
            message = self.error_message
        self.scores_message.set(message)

        # process word entered
    def word_entered(self, event):
        '''
        Get trial word inputted to input widget
        If game over, accept y or n to restart or end game.
        '''
        if self.game_end:
            if self.trial_word.get() == 'y' or self.trial_word.get() == 'Y':
                self.re_start()
            elif self.trial_word.get() == 'n' or self.trial_word.get() == 'N':
                exit(0)
            else:
                self.reset_trial_word()

        if not self.trial_word.get() == '':
            self.game_play(self.trial_word.get())

    def game_play(self, guess):
        """
        Accept trial word, process it,
            update scores, update found_words list, update player_num
        """
        if guess == 'first_guess':  # Needed because startup sends a '' first word
            player_name = self.players[0][0]
            message = player_name + ', enter a word'
            self.player_name_message.set(message)

        else:
            guess = guess.strip().upper()
            if a3.is_valid_word(self.words, guess) and a3.board_contains_word(self.board, guess) and \
                    not guess in self.found_words_list:
                a3.update_score(self.players[self.player_num], guess)

                self.set_scores()
                self.found_words_list.append(guess)
                self.set_found_words(self.found_words_list)

                num_remaining = a3.num_words_on_board(self.board, self.words) - len(self.found_words_list)
                self.set_num_words_rem(num_remaining)
                self.reset_trial_word()
                if num_remaining == 0:
                    self.game_over()
                else:
                    self.player_num += 1
                    if self.player_num >= len(self.players):
                        self.player_num = 0
                    self.set_player_name(self.player_num)

            else:
                self.error_message = "\"" + guess + "\"" + " invalid, not in board, or previously chosen. Miss turn."
                self.invalid_word = True
                self.set_scores()
                self.player_num += 1
                if self.player_num >= len(self.players):
                    self.player_num = 0
                self.set_player_name(self.player_num)
                self.reset_trial_word()

        # if game  over, announce winner & options
    def game_over(self):
        '''
        sort player list by scores, provide winner for  set.label text,
        and send play another message to player name label
        '''
                #sort player list by scores, winner is last(highest) element
        self.winner = sorted(self.players, key = lambda x: int(x[1]))[-1][0]
        self.game_end = True
        self.set_scores() #announce winner
        message = "Play another? y/n"
        self.set_player_name(0, message)

    #  Initial setup steps needed for restart
    def re_start(self):
        '''
        Initialize conditions for a restart
        '''
        message = ''
        self.reset_trial_word()
        self.player_name_message.set(message)
        self.game_end = False
        self.players = []
        self.found_words_list = []
        self.set_found_words(self.found_words_list)
        self.player_num = 0
        self.text_box['state'] = 'normal'
        self.text_box.delete('1.0', 'end')
        self.text_box['state'] = 'disabled'
        self.scores_message.set('')
        self.start_game()

    def start_game(self):
        '''
         these startup steps open new temporary frames
        '''

        self.open_board()
        self.get_words()
        self.get_players()

        #with players in place, begin play
    def start_play(self):
        '''
        Once players, wordlist and board inputted,
        commence play
        '''
        self.set_scores()
        self.word_entry.focus_set()
        self.game_play("first_guess")

        #open board & send it to text frame
    def open_board(self):
        '''
        Get user to select board, read it in via a3 module
        '''
        board_file = askopenfile(mode='r', title='Select board file')
        self.board = a3.read_board(board_file)
        self.draw_board(self.board)
        board_file.close()


    def draw_board(self, board):
        '''
        Accept board & copy it to text frame.   This involves deep-copying it, reversing inside lists
        and order of lists, because text.insert() method inserts (pushes earlier characters to right)
        '''
        # TODO: experimant with inserting beginning with list[-1][-1]
        # TODO: animation to highlight chosen word

        self.text_box['state'] = 'normal'
        board_rev = []
        for row in board:
            board_rev.append(row[:])
        for row in board_rev:
            row.reverse()
            row.append('\n')
        board_rev.reverse()
        for row in board_rev:
            for ch in row:
                self.text_box.insert('1.0', ch + ' ')
        self.text_box['state'] = 'disabled' # Player can't change text

        #load words_file
    def get_words(self):
        '''
        Get user to select word list, read it in via a3 module
        '''
        # TODO:  Actually create a list of words within board (not just number of valid words)
        # TODO (cont'd) (probably need to do this outside of a3.py) - offer an option to show the list
        words_file = askopenfile(mode='r', title='Select word list file')
        self.words = a3.read_words(words_file)
        words_file.close()
        self.set_num_words_rem(a3.num_words_on_board(self.board, self.words))

    def get_players(self):
        '''
        Set up frame for player input
        '''
        #TODO Consider separate players class
        # text variables
        self.next_player_text = StringVar()
        self.player_name_text = StringVar()
        first_player_message = "Type name of player " + str(len(self.players) + 1) + " then press 'Enter' "
        # widgets
        self.entry_frame = Frame(self.root, borderwidth=5, relief="sunken", width=200, height=200)
        self.name_label_1 = Label(self.entry_frame, textvariable = self.next_player_text)
        name_label_2 = Label(self.entry_frame, text="Leave blank and press 'Enter' to play")
        name = Entry(self.entry_frame, width=25, textvariable = self.player_name_text)
        # Grid positions
        self.entry_frame.grid(column=0, row=0)
        self.name_label_1.grid(column=0, row=1, columnspan=2)
        name_label_2.grid(column=0, row=2, columnspan=2)
        name.grid(column=0, row=3, columnspan=2)
        #action
        self.next_player_text.set(first_player_message)
        name.bind('<Key-Return>', self.name_entered)


    def name_entered(self, event):
        '''
        Accept new player name, append to player list; commence game
        and close out frame if enter pressed on blank form.
        '''
        player = self.player_name_text.get()
        if not player == "":
            player = player.strip()

            if [player, 0] in self.players:
                message = "A player by that name is already playing."
                self.next_player_text.set(message)
                self.player_name_text.set('')

            else:
                self.players.append([player, 0])
                message = "Type name of player " + str(len(self.players) + 1) + " then press 'Enter' "
                self.next_player_text.set(message)
                self.player_name_text.set('')
        else:
            if len(self.players) == 0:
                self.players = [["Player", 0]]
            self.entry_frame.forget()
            self.entry_frame.destroy()
            self.start_play()

def main():

    root = Tk()
    root.resizable(width=True, height=True)
    app = WordGui(root)
    root.mainloop()

if __name__ == '__main__':
    main()