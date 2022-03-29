#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define MAX 9


int preferences[MAX][MAX];
bool locked[MAX][MAX];


typedef struct
{
    int winner;
    int loser;
}
pair;


char candidates[MAX][50];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;


bool vote(int rank, char name[50], int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
	return 1;
    }


    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
	return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        strcpy(candidates[i],argv[i+1]);
    }


    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
	{
	   locked[i][j] = false;
	}
	}

    pair_count = 0;
    int voter_count;
    printf("Number of voters\n");
    scanf("%i",&voter_count);


    for (int i = 0; i < voter_count; i++)
    {

        int ranks[candidate_count];


        for (int j = 0; j < candidate_count; j++)
        {
	    char name[50];
	    printf("Rank %i:",j+1);
	    scanf("%s",name);

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


bool vote (int rank, char name[50], int ranks[])
{

    for (int i = 0; i < candidate_count; i++)
    {
        if(strcmp(candidates[i],name) == 0)
        {
	      ranks[rank] = i;
	      return true;
	}
    }
    return false;
}


void record_preferences(int ranks[])
{

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
	        preferences[ranks[i]][ranks[j]] += 1;
	}
    }
    return;
}


void add_pairs(void)
{

    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
	       if (preferences[i][j] > preferences[j][i])
	       {
		    pairs[pair_count].winner = i;
		    pairs[pair_count].loser = j;
		    pair_count++;
		}
		else if (preferences[i][j] < preferences[j][i])
		{
		    pairs[pair_count].winner = j;
		    pairs[pair_count].loser = i;
		    pair_count++;
		}
	}
    }
	return;
}


void sort_pairs(void)
{

    for (int i = 0; i < pair_count; i++)
    {
        int max = i;
	    for (int j = i + 1; j < pair_count; j++) {
	        if (preferences[pairs[j].winner][pairs[j].loser] > preferences[pairs[max].winner][pairs[max].loser])
		{
		        max = j;
		}
		}
		pair temp = pairs[i];
		pairs[i] = pairs[max];
		pairs[max] = temp;
    }
    return;
}

bool is_circle(int loser, int winner)
{

    if (loser == winner) {
		return true; 
	}

    for (int i = 0; i < candidate_count; i++)
    {
        if(locked[loser][i]) 
        {
	        return is_circle(i, winner); 
	}
    }

	return false;
}


void lock_pairs(void)
{

    for (int i = 0; i < pair_count; i++)
    {
        if (!is_circle(pairs[i].loser, pairs[i].winner))
        {
           locked[pairs[i].winner][pairs[i].loser] = true;
	}

    }
    return;
}


void print_winner(void)
{

    for (int i = 0; i < candidate_count; i++)
    {
        bool isLoser = false;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
	    {
               isLoser = true;
               break;
            }

        }

        if (isLoser) continue;
        if(!isLoser) 
        {
            printf("The winner is %s\n", candidates[i]);
        }
    }
	return;
}
