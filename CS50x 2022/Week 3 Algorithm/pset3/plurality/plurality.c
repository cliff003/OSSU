#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        // Validate the vote, add one vote if valid
        if (strcmp(candidates[i].name, name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // Array of Winners
    candidate winner[candidate_count];
    winner[0].votes = -1;

    // Set Winner count
    int winner_count = 0;

    // Check the votes
    for (int i = 0; i < candidate_count; i++)
    {
        // Find the maximum votes
        if (winner[0].votes < candidates[i].votes)
        {
            winner[0].name = candidates[i].name;
            winner[0].votes = candidates[i].votes;
        }

        // Add a winner count if same votes
        else if (winner[0].votes == candidates[i].votes)
        {
            if (winner[winner_count].votes < winner[0].votes)
            {
                // Reset winner count if find new maximum
                winner_count = 0;
            }
            winner[winner_count + 1].name = candidates[i].name;
            winner[winner_count + 1].votes = candidates[i].votes;
            winner_count++;
        }
    }

    // Print the Winners
    for (int j = 0; j < winner_count + 1; j++)
    {
        printf("%s\n", winner[j].name);
    }
}