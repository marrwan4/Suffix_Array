#include<iostream>

using namespace std;

class SuffixArray
{
private:
    // Structure to store information of a suffix and its ranks
    struct Entry
    {
        int ind;
        int r1;
        int r2;
    };
    char* array;
    int* suffixArray;
    int length;
    // Function to get length of string
    int getLength(const char* str)
    {
        int len = 0;
        while (str[len] != '\0')
        {
            len++;
        }
        return len;
    }
    // copy string from source to destination
    void copyString(const char* src, char* dest)
    {
        int i = 0;
        while (src[i] != '\0')
        {
            dest[i] = src[i];
            i++;
        }
        dest[i] = '\0';
    }

    // Comparator pointer type
    typedef bool (*comparatorFunction)(const Entry&, const Entry&);

    // Compare by (r1, r2)
    static bool cmp_r1r2(const Entry &a, const Entry &b) {
        if (a.r1 < b.r1)
        {
            return true;
        }
        if (a.r1 > b.r1)
        {
            return false;
        }
        return a.r2 < b.r2;
    }

    // Compare by index
    static bool cmp_ind(const Entry &a, const Entry &b) {
        return a.ind < b.ind;
    }

    // Merger
    void merge(Entry arr[], Entry temp[], int left, int mid, int right, comparatorFunction cmp)
    {
        int i = left;
        int j = mid + 1;
        int k = left;
        // Merge the two halves into temp[]
        while (i <= mid && j <= right)
        {
            if (cmp(arr[i], arr[j]))
            {
                temp[k] = arr[i];
                k++;
                i++;
            }
            else
            {
                temp[k] = arr[j];
                k++;
                j++;
            }
        }
        // Copy remaining elements of left half, if any
        while (i <= mid)
        {
            temp[k] = arr[i];
            k++;
            i++;
        }
        // Copy remaining elements of right half, if any
        while (j <= right)
        {
            temp[k] = arr[j];
            k++;
            j++;
        }
        // Copy the merged subarray into original array
        for (int x = left; x <= right; x++)
        {
            arr[x] = temp[x];
        }
    }

    // Recursive merge sort
    void mergeSort(Entry arr[], Entry temp[], int left, int right, comparatorFunction cmp)
    {
        if (left >= right) 
        {
            return;
        }
        int mid = (left + right) / 2;
        mergeSort(arr, temp, left, mid, cmp);
        mergeSort(arr, temp, mid + 1, right, cmp);
        merge(arr, temp, left, mid, right, cmp);
    }

    // Wrapper
    void mergeSort(Entry arr[], int n, comparatorFunction cmp)
    {
        Entry* temp = new Entry[n];
        mergeSort(arr, temp, 0, n - 1, cmp);
        delete[] temp;
    }
public:
    SuffixArray(const char* input)
    {
        length = getLength(input);
        // Allocate memory and copy input string to array
        array = new char[length + 1];
        copyString(input, array);
        // Allocate memory for suffix array
        suffixArray = new int[length];
    }
    void ConstructUsingPrefixDoubling()
    {
        // Allocate array of Entry structures to hold suffix information
        Entry* entries = new Entry[length];
        
        // Step 1: Initialize entries with index and initial ranks (k=0)
        // At k=0, we consider only the first character of each suffix
        for (int i = 0; i < length; i++)
        {
            // Set the index of the suffix
            entries[i].ind = i;
            // Set r1 as the ASCII value of the first character
            entries[i].r1 = array[i];
            // Set r2 as the ASCII value of the next character (or -1 if none)
            if (i + 1 < length)
            {
                entries[i].r2 = array[i + 1];
            }
            else
            {
                entries[i].r2 = -1; // No next character
            }
        }
        
        // Step 2: Sort entries by (r1, r2) pairs
        mergeSort(entries, length, cmp_r1r2);
        
        // Step 3: Assign new ranks based on sorted order
        // Allocate temporary array to store new ranks
        int* tempRanks = new int[length];
        // First suffix in sorted order gets rank 0
        tempRanks[entries[0].ind] = 0;
        
        // Assign ranks: same rank if (r1, r2) pairs are equal, otherwise increment
        for (int i = 1; i < length; i++)
        {
            // Check if current entry has same (r1, r2) as previous entry
            if (entries[i].r1 == entries[i - 1].r1 && entries[i].r2 == entries[i - 1].r2)
            {
                // Same rank as previous suffix
                tempRanks[entries[i].ind] = tempRanks[entries[i - 1].ind];
            }
            else
            {
                // New rank (previous rank + 1)
                tempRanks[entries[i].ind] = tempRanks[entries[i - 1].ind] + 1;
            }
        }
        
        // Update r1 with new ranks for next iteration
        for (int i = 0; i < length; i++)
        {
            entries[i].r1 = tempRanks[entries[i].ind];
        }
        
        // Step 4: Prefix doubling - repeat for k = 1, 2, 4, 8, ...
        // k represents the number of characters we've already ranked (doubles each iteration)
        for (int k = 1; k < length; k *= 2)
        {
            // Sort entries by index to prepare for updating r2
            mergeSort(entries, length, cmp_ind);
            
            // Update r2 to be the rank of suffix starting at position (ind + k)
            // This allows us to compare 2k characters using two k-character comparisons
            for (int i = 0; i < length; i++)
            {
                // r1 already contains rank of first k characters
                // r2 should contain rank of next k characters
                if (i + k < length)
                {
                    // Rank of suffix starting at (i + k) is stored at entries[i + k].r1
                    entries[i].r2 = entries[i + k].r1;
                }
                else
                {
                    // No suffix k positions ahead, use -1
                    entries[i].r2 = -1;
                }
            }
            
            // Sort entries by (r1, r2) pairs to get new ordering
            mergeSort(entries, length, cmp_r1r2);
            
            // Assign new ranks based on sorted order
            tempRanks[entries[0].ind] = 0;
            for (int i = 1; i < length; i++)
            {
                // Check if current entry has same (r1, r2) as previous entry
                if (entries[i].r1 == entries[i - 1].r1 && entries[i].r2 == entries[i - 1].r2)
                {
                    // Same rank as previous suffix
                    tempRanks[entries[i].ind] = tempRanks[entries[i - 1].ind];
                }
                else
                {
                    // New rank (previous rank + 1)
                    tempRanks[entries[i].ind] = tempRanks[entries[i - 1].ind] + 1;
                }
            }
            
            // Update r1 with new ranks for next iteration
            for (int i = 0; i < length; i++)
            {
                entries[i].r1 = tempRanks[entries[i].ind];
            }
        }
        
        // Step 5: Final sort by ranks to ensure suffix array is in correct order
        mergeSort(entries, length, cmp_r1r2);
        
        // Step 6: Extract suffix array (just the indices in sorted order)
        for (int i = 0; i < length; i++)
        {
            suffixArray[i] = entries[i].ind;
        }
        
        // Free allocated memory
        delete[] tempRanks;
        delete[] entries;
    }
    // Print suffix array
    void Print()
    {
        // for each suffix in suffix array print its starting index
        for (int i = 0; i < length; i++)
        {
            cout << suffixArray[i] << " ";
        }
        cout << endl;
    }
    // Destructor to free allocated memory
    ~SuffixArray()
    {
        delete[] array;
        delete[] suffixArray;
    }
};

int main()
{
    SuffixArray t("ACGACTACGATAAC$");

    t.ConstructUsingPrefixDoubling();

    t.Print(); // Prints:  14 11 12  0  6  3  9 13  1  7  4  2  8 10  5

    return 0;
}

// The following is just illustration for your help only, nothing required about it. Check lecture.
///////////////////////////////////////////////////////////////////////////////////////////////////
// i      0  1  2  3  4  5  6  7  8  9 10 11 12 13 14
// t      A  C  G  A  C  T  A  C  G  A  T  A  A  C  $
// --------------------------------------------------
// k=0    1  2  3  1  2  4  1  2  3  1  4  1  1  2  0
// k=1    2  5  7  2  6  8  2  5  7  3  8  1  2  4  0
// k=2    3  7 10  4  9 13  3  8 11  5 12  1  2  6  0
// k=4    3  8 11  5 10 14  4  9 12  6 13  1  2  7  0
// sa=   14 11 12  0  6  3  9 13  1  7  4  2  8 10  5
///////////////////////////////////////////////////////////////////////////////////////////////////