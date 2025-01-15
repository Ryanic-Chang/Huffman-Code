#include "linked_list.h"
#include "huffman.h"
#include "encode.h"
#include "decode.h"
#include "frequency.h"
#include "verify.h"
#include "match.h"
#include "word_count.h"
#include "fuzzy_match.h"
#include "generate.h"
#include "read.h"
#include <stdio.h>
#include <stdlib.h>

void displayMenu() {
    printf("\n--- Menu ---\n");
    printf("1. Calculate Character Frequency\n");
    printf("2. Generate Huffman Codes\n");
    printf("3. Encode File\n");
    printf("4. Decode File\n");
    printf("5. Compare Original and Decoded Files\n");
    printf("6. Match Word in File\n");
    printf("7. Count Words\n");
    printf("8. Fuzzy Match in File\n");
    printf("9. Update CharList by Word\n");
    printf("10. Generate Huffman Codes by Word\n");
    printf("11. Encode File by Word\n");
    printf("12. Decode File by Word\n");
    printf("13. Compare Original and Decoded Files by Word\n");
    printf("14. Text Summary\n");
    printf("15. Graph Generate\n");
    printf("16. Video Generate\n");
    printf("17. Chat With AI\n");
    printf("0. Exit\n");
    printf("Choose an option: ");
}

int main() {
    ListNode* charList = NULL;// 存储单词和频率的链表
    ListNode* charList_Word = NULL;
    HuffmanTreeNode* root = NULL;
    HuffmanTreeNode* root_Word = NULL;
    char** code = NULL;
    int character[MAX_CHAR_COUNT];
    char** code_Word = NULL;
    int character_Word[MAX_CHAR_COUNT];
    int countNode = 0;
    int countNode_Word = 0;
    int option;
    char pattern[100];
    printf("Make sure your SourceDoc.txt file is in the data directory.");
    while (1) {
        displayMenu();
        scanf("%d", &option);
        getchar(); // Consume newline character after option
        
        switch (option) {
            case 1:
                calculateFrequency("data/SourceDoc.txt", &charList);
                countNode = countList(charList);
                printf("Character Frequency saved.\n");
                break;
                
            case 2:
                if (charList == NULL) {
                    printf("Calculate frequencies first (Option 1).\n");
                    break;
                }
                root = (HuffmanTreeNode*) malloc(2 * countNode * sizeof(HuffmanTreeNode));
                initHuffmanTree(root, charList, countNode);
                createHuffmanTree(root, countNode);
                createHuffmanCode(root, &code, countNode);
                storeHuffmanCode("output/HuffmanCode.txt", code, root, countNode);
                printf("Huffman Codes generated and saved.\n");
                break;
                
            case 3:
                if (code == NULL || root == NULL) {
                    printf("Generate Huffman codes first (Option 2).\n");
                    break;
                }
                readFile("output/HuffmanCode.txt", character);
                encodeFile("data/SourceDoc.txt", "output/Encode.dat", code, root, countNode, character);
                printf("File encoded successfully.\n");
                break;
                
            case 4:
                if (root == NULL) {
                    printf("Generate Huffman codes first (Option 2).\n");
                    break;
                }
                readFile("output/HuffmanCode.txt", character);
                decodeFile("output/Encode.dat", "output/Target.txt", root, countNode, character);
                printf("File decoded successfully.\n");
                break;
                
            case 5:
                compareFiles("data/SourceDoc.txt", "output/Target.txt");
                break;
                
            case 6:
                printf("Enter the word or phrase to search: ");
                fgets(pattern, sizeof(pattern), stdin);
                pattern[strcspn(pattern, "\n")] = 0; // Remove newline
                searchParagraph(pattern);
                break;
                
            case 7:
                countWords("data/SourceDoc.txt", "output/WordCount.txt");
                //printf("%d\n",wordCount);
                printf("Word count completed and saved.\n");
                break;
                
            case 8:
                printf("Enter the word or phrase to fuzzy match: ");
                fgets(pattern, sizeof(pattern), stdin);
                pattern[strcspn(pattern, "\n")] = 0; // Remove newline
                fuzzyMatchInFile(pattern);
                break;

            case 9:
                countWords("data/SourceDoc.txt", "output/WordCount.txt");
                calculateFrequency("data/SourceDoc.txt", &charList_Word);
                deleteAlphaNodes(&charList_Word);
                updateCharList(&charList_Word,sortedWords);
                countNode_Word = countList(charList_Word);
                printf("CharList updated.\n");
                //printCharList(charList_Word);
                break;
                
            case 10:
                if (charList_Word == NULL) {
                    printf("Calculate frequencies first (Option 9).\n");
                    break;
                }
                root_Word = (HuffmanTreeNode*) malloc(2 * countNode_Word * sizeof(HuffmanTreeNode));
                initHuffmanTree(root_Word, charList_Word, countNode_Word);
                createHuffmanTree(root_Word, countNode_Word);
                createHuffmanCode(root_Word, &code_Word, countNode_Word);
                storeHuffmanCode("output/HuffmanCode_Word.txt", code_Word, root_Word, countNode_Word);
                printf("Huffman Codes generated and saved.\n");
                break;

            case 11:
                if (code_Word == NULL || root_Word == NULL) {
                    printf("Generate Huffman codes first (Option 10).\n");
                    break;
                }
                readFile_Word("output/HuffmanCode_Word.txt", character_Word);
                encodeFile_Word("data/SourceDoc.txt", "output/Encode_Word.dat", code_Word, root_Word, countNode_Word, character_Word);
                printf("File encoded successfully.\n");
                break;

            case 12:
                if (root_Word == NULL) {
                    printf("Generate Huffman codes first (Option 11).\n");
                    break;
                }
                readFile_Word("output/HuffmanCode_Word.txt", character_Word);
                decodeFile_Word("output/Encode_Word.dat", "output/Target_Word.txt", root_Word, countNode_Word, character_Word);
                printf("File decoded successfully.\n");
                break;
            
            case 13:
                compareFiles("data/SourceDoc.txt", "output/Target_Word.txt");
                break;

            case 14:
                text_generation("data/SourceDoc.txt");
                break;

            case 15:
                image_generation("data/Graph.txt");
                break;

            case 16:
                video_generation("data/Video.txt");
                break;

            case 17:
                chat("data/SourceDoc.txt");
                break;
            
            case 0:
                printf("Exiting program.\n");
                return 0;
                
            default:
                printf("Invalid option, please try again.\n");
        }
    }

    return 0;
}