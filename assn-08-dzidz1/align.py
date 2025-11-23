#!/usr/bin/env python

import random  # for seed, random
import sys     # for stdout


################################### TEST PART ##################################
################################################################################

# Tests align strands and scores
# Parameters types:
#    score          =  int   example: -6
#    plusScores     = string example: "  1   1  1"
#    minusScores    = string example: "22 111 11 "
#    strandAligned1 = string example: "  CAAGTCGC"
#    strandAligned2 = string example: "ATCCCATTAC"
#
#   Note: all strings must have same length
def test(score, plusScores, minusScores, strandAligned1, strandAligned2):
    print("\n>>>>>>START TEST<<<<<<")

    maxLength = max(len(plusScores),
                    len(minusScores), len(strandAligned1), len(strandAligned2))
    plusScores = plusScores.ljust(maxLength)
    minusScores = minusScores.ljust(maxLength)
    strandAligned1 = strandAligned1.ljust(maxLength)
    strandAligned2 = strandAligned2.ljust(maxLength)

    if testStrands(score, plusScores, minusScores, strandAligned1, strandAligned2):
        sys.stdout.write(">>>>>>>Test SUCCESS:")
        sys.stdout.write("\n\t\t" + "Score: "+str(score))
        sys.stdout.write("\n\t\t+ " + plusScores)
        sys.stdout.write("\n\t\t  " + strandAligned1)
        sys.stdout.write("\n\t\t  " + strandAligned2)
        sys.stdout.write("\n\t\t- " + minusScores)
        sys.stdout.write("\n\n")
    else:
        sys.stdout.write("\t>>>>!!!Test FAILED\n\n")


# Converts character score to int
def testScoreToInt(score):
    if score == ' ':
        return 0
    return int(score)


# Computes sum of scores
def testSumScore(scores):
    result = 0
    for ch in scores:
        result += testScoreToInt(ch)
    return result


# Test each character and scores
def testValidateEach(ch1, ch2, plusScore, minusScore):
    if ch1 == ' ' or ch2 == ' ':
        return plusScore == 0 and minusScore == 2
    if ch1 == ch2:
        return plusScore == 1 and minusScore == 0
    return plusScore == 0 and minusScore == 1


# Test and validates strands
def testStrands(score, plusScores, minusScores, strandAligned1, strandAligned2):
    if len(plusScores) != len(minusScores) or len(minusScores) != len(strandAligned1) or len(strandAligned1) != len(strandAligned2):
        sys.stdout.write("Length mismatch! \n")
        return False

    if len(plusScores) == 0:
        sys.stdout.write("Length is Zero! \n")
        return False

    if testSumScore(plusScores) - testSumScore(minusScores) != score:
        sys.stdout.write("Score mismatch to score strings! TEST FAILED!\n")
        return False
    for i in range(len(plusScores)):
        if not testValidateEach(strandAligned1[i], strandAligned2[i], testScoreToInt(plusScores[i]),
                                testScoreToInt(minusScores[i])):
            sys.stdout.write("Invalid scores for position " + str(i) + ":\n")
            sys.stdout.write("\t char1: " + strandAligned1[i] + " char2: " +
                             strandAligned2[i] + " +" + str(testScoreToInt(plusScores[i])) + " -" +
                             str(testScoreToInt(minusScores[i])) + "\n")
            return False

    return True

######################## END OF TEST PART ######################################
################################################################################


# Computes the score of the optimal alignment of two DNA strands.
def findOptimalAlignment(mem, strand1, strand2):
    key = (strand1, strand2)
    if key in mem:
        return mem[key]

    if len(strand1) == 0:
        aligned1 = " " * len(strand2)
        aligned2 = strand2
        score = len(strand2) * -2
        mem[key] = (score, aligned1, aligned2)
        return mem[key]

    if len(strand2) == 0:
        aligned1 = strand1
        aligned2 = " " * len(strand1)
        score = len(strand1) * -2
        mem[key] = (score, aligned1, aligned2)
        return mem[key]

    scoreWith, align1With, align2With = findOptimalAlignment(mem, strand1[1:], strand2[1:])
    if strand1[0] == strand2[0]:
        scoreWith += 1
    else:
        scoreWith -= 1

    align1With = strand1[0] + align1With
    align2With = strand2[0] + align2With

    scoreWithout2, align1Without2, align2Without2 = findOptimalAlignment(mem, strand1[1:], strand2)
    scoreWithout2 -= 2
    align1Without2 = strand1[0] + align1Without2
    align2Without2 = " " + align2Without2

    scoreWithout1, align1Without1, align2Without1 = findOptimalAlignment(mem, strand1, strand2[1:])
    scoreWithout1 -= 2
    align1Without1 = " " + align1Without1
    align2Without1 = strand2[0] + align2Without1

    if scoreWith >= scoreWithout2 and scoreWith >= scoreWithout1:
        mem[key] = (scoreWith, align1With, align2With)
    elif scoreWithout2 >= scoreWith and scoreWithout2 >= scoreWithout1:
        mem[key] = (scoreWithout2, align1Without2, align2Without2)
    else:
        mem[key] = (scoreWithout1, align1Without1, align2Without1)

    return mem[key]


# Utility function that generates a random DNA string of
# a random length drawn from the range [minlength, maxlength]
def generateRandomDNAStrand(minlength, maxlength):
    assert minlength > 0, \
        "Minimum length passed to generateRandomDNAStrand must be a positive number"
    assert maxlength >= minlength, \
        "Maximum length passed to generateRandomDNAStrand must be at least as large as the specified minimum length"
    strand = ""
    length = random.choice(xrange(minlength, maxlength + 1))
    bases = ['A', 'T', 'G', 'C']
    for i in xrange(0, length):
        strand += random.choice(bases)
    return strand


# Method that just prints out the supplied alignment score.
# This is more of a placeholder for what will ultimately
# print out not only the score but the alignment as well.
def printAlignment(score, out=sys.stdout):
    out.write("Optimal alignment score is " + str(score) + "\n")



def main():
    while True:
        user_input = raw_input("Generate random DNA strands? ").strip().lower()
        if user_input == 'no':
            break
        elif user_input == 'yes':
            strand1 = generateRandomDNAStrand(5, 12)
            strand2 = generateRandomDNAStrand(5, 12)
            print("Aligning these two strands:")
            print(strand1)
            print(strand2)

            mem = {}
            score, aligned1, aligned2 = findOptimalAlignment(mem, strand1, strand2)

           
            plusScores = "".join(
                "1" if aligned1[i] == aligned2[i] and aligned1[i] != " " else " "
                for i in range(len(aligned1))
            )
            minusScores = "".join(
                "2" if aligned1[i] == " " or aligned2[i] == " " else
                "1" if aligned1[i] != aligned2[i] else " "
                for i in range(len(aligned1))
            )

            print "Optimal alignment score is " + str(score)

            test(score, plusScores, minusScores, aligned1, aligned2)


if __name__ == "__main__":
    main()
