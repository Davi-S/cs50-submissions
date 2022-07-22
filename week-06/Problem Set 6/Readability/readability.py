def main():
    text = input("TEXT: ")

    # Get amount of each item to check on the text
    letters = count_letters(text)
    words = count_words(text)
    sentences = sentence_count(text)

    # Amount per word
    letters_p_w = letters / words * 100.0
    sentences_p_w = sentences / words * 100.0

    # Coleman-Liau formula
    index = get_index(letters_p_w, sentences_p_w)

    # Print results
    if 1 < index < 16:
        print(f"Grade {index}")
    if index > 16:
        print(f"Grade 16+")
    elif index < 1:
        print(f"Before Grade 1")


def count_letters(text: str):
    count = 0
    for i in text:
        if i.isalpha():
            count += 1
    return count


def count_words(text: str):
    return text.count(' ') + 1  # Adding one for the last word


def sentence_count(text: str):
    count = 0
    for i in ['.', '!', '?']:
        count += text.count(i)
    return count


def get_index(letters_p_w, sentences_p_w):
    index = 0.0588 * letters_p_w - 0.296 * sentences_p_w - 15.8
    return round(index)


if __name__ == "__main__":
    main()
