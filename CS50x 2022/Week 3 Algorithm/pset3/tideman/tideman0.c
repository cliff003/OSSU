#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
    int strength;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;


// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        // Validate the vote
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Record in array
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1 ; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1 ; j < candidate_count; j++)
        {
            // Compare and record pairs
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i, pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j, pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Add strength to pairs
    for (int i = 0; i < pair_count; i++)
    {
        pairs[i].strength = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
    }

    // Sort in decreasing order of strength by using bubble sort
    pair temp[1];
    for (int j = 0; j < pair_count - 1; j++)
    {
        for (int k = 0; k < pair_count - 1 - j; k++)
        {
            // Swap pairs
            if (pairs[k].strength < pairs[k + 1].strength)
            {
                temp[0] = pairs[k];
                pairs[k] = pairs[k + 1];
                pairs[k + 1] = temp[0];
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Lock pairs into the graph
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;

        // Check the edges would create a cycle.(Ugly but useful xD)
        for (int j = 0, m = 0, o = 0; j < candidate_count; j++)
        {
            for (int k = 0, n = 0; k < candidate_count; k++)
            {
                // Count non-edges
                if (locked[j][k] == false)
                {
                    n++;
                }

                //Count edges
                else if (locked[j][k] == true)
                {
                    o++;
                }

                // Reset m if no edge for a cycle
                if (n == candidate_count)
                {
                    m = 0;
                }
            }

            // Count edges
            m++;

            // When there is an edge for a cycle, reset it
            if (m == candidate_count || o == candidate_count)
            {
                locked[pairs[i].winner][pairs[i].loser] = false;
            }
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int winner[candidate_count];
    // Find the source of graph
    for (int j = 0; j < candidate_count; j++)
    {
        winner[j] = 0;
        for (int k = 0; k < candidate_count; k++)
        {
            if (locked[k][j] == false)
            {
                winner[j]++;
            }
        }
    }

    // Print final winner
    for (int i = 0; i < candidate_count; i++)
    {
        if (winner[i] == candidate_count)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}