/******************************************************************************
* Name  : sum.c
* Author: Julia Nelson
* Date  : June 24, 2021
* Pledge: "I pledge my honor that I have abided by the Stevens Honor System."
* Description: Lab 12 - Dynamically Linked Libraries
******************************************************************************/
#include "sum.h"

/**
 * TODO:
 * Takes in an array of integers and its length.
 * Returns the sum of integers in the array.
 */

int sum_array(int *array, const int length) {
	int sum = 0;
	for (int i = 0; i < length; i++) {
		sum = sum + array[i];
	}
	return sum;

}
