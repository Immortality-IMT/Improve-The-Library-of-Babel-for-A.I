# Improve the Library of Babel for A.I.
Improve Borges Library of Babel to take into account recent advances in A.I. that can possibly organise the data to finally be useful.

These files generate the library sequentially into a database. They have two functions, one to omit data based on regular expressions and another to send the data to GPT to make decisions on the data.

Improving the library of babel to find cures to aging.

Compile with: 
gcc -g -o anyname source_file.c -lsqlite3
e.g.
gcc -g -o babel dictionary.c -lsqlite3

# Knighinggale Mason writes...

A thought on AI and the Library of Babel...

Curious if anyone knows about projects with AI sorting through the data in the Library of Babel.

For example, in art, we can have AI take similar images and find the differences between each. As well as similarities.

Could this be applied to the "static" in the library? In other words, have my computer find similarities between the hex keys of two nearly identical writings (or images) then have the AI find alternatives?

If it hasn't been done, then perhaps that should be looked into? I'm not sure who to even contact about such a project.

You know, at Immortality, there is no place we will not tread in order to find cures. Is the library of babel hiding some valuable information? 

# What is the Library of Babel?

The library of babel was first mentioned in a short story by Jorge Luis Borges: "The Library of Babel" in 1941 describes a vast and infinite library containing every possible combination of letters, resulting in every book that has ever has been or could ever be written, including complete gibberish.

By representing every possible combination of letters you get every word possible, by concatenating every combination of words you get every piece of information possible.

Would infinite books give us infinite knowledge? Every idea, every invention, every theory and fact will be contained somewhere in the infinity of the combinations of words. Unwritten works and sequels to Shakespeare, interstellar propulsion systems and more.

Yet only a small number of books contain valuable information obfuscated by the sheer amount of possibilities, and there is the challenge. Like an artist sculptor that begins with a clump of clay, we must forge the sculpture, being certain not to cut away any valuable information. There is little to no human capacity for serendipity or discovery. A needle in a very large haystack, finding a page that describes something important is lost in a mountain of pages beyond human capacity to find.

Is the library of babel one of those interesting yet useless things, or can we improve it further and make it useful?

"The Library of Babel" was then turned into a website (https://libraryofbabel.info/) that emulates the concept of Borges' library. It uses an algorithm to generate random "books" with 3200 characters each, containing all possible permutations of 29 characters (26 letters, space, comma, and period). This means the website contains approximately 10^4677 potential pages of books, even though most are nonsensical. The website allows users to browse the virtual library by entering coordinates like hexagon names, wall and shelf numbers, and book titles, a kind of Dewey Decimal Classification (DDC) system. 

# Challenge: Improve the Library of Babel.

OK. Challenge Accepted.

So the library of babel needs curation, a librarian to direct us to an amount of results, narrow down the results within a human capacity.

An essential deviation from the puritan philosophical concept of the library is to use textual analysis to identify and omit gibberish. Perhaps check all the words against various dictionaries, omitting words that do not exist at least "yet".

Further on apply language rules, grammar rules, so that only valid sentences remain, making the library human-readable and understandable.
Advanced textual analysis has the capacity to sample large amounts of text and determine how sensical that data is, further improving overall readability.

Checking for N-gram frequency and entropy calculation.

Severe textual analysis can produce a human-readable and understandable library, and we can endlessly tweak, re-evaluate and regenerate the library as required. The library would then exhibit valid words with readable sentences and readable pages. How much information was lost and was it an acceptable tradeoff? Language buffs and logic buffs would probably find this sort of thing interesting. Textual analysis is essential and would likely be implemented in the programming stage.

Some issues are... 

The trade-off between readability and loss of information.

Incorrectly omitting words, sentences that are either coined sometime in the future but currently appear to have no meaning. A word needs to enter the lexicon before it has meaning, context and valid text could falsely appear nonsensical but could just beyond human understanding. If a generated word is not in the dictionary then it could be gibberish, slang or future slang. 

Permutational exhaustiveness would classify unbounded repetition such as Buffalo buffalo buffalo Buffalo buffalo as valid, well programmed grammar rules are able to correctly identify such valid sentences from invalid sentences. 

The longest word in the English language is 45 characters. The current longest valid sentence is 13,955 words long. A page of text usually contains 500 words, the longest book is 21,450 Pages. The library could generate a longer valid sentence and a longer sensical book.

There are several versions of the library of babel online that can be downloaded to desktop.
These often use a random function to generate pages rather than being sequential.

Building a real version of the library follows...

Step 1: Generate the dictionary for the library. Its format could serve as part of the libraries' website. Complete with definitions.

Step 2: Use that dictionary to generate sentences of varying lengths, all the 2-word sentences, 3-word sentences and so on... The possibility of a dictionary of sentences also mapped to definitions. Humans probably stopped at a dictionary prior to computers, even though a dictionary of sentences is possible. A dictionary of sentence could also be part of the library website.

Sentences lead to paragraphs... certain paragraphs are definitions for the word, but generally paragraphs lead to pages, pages to books, books to volumes and finally the library. 

Each time the library is generated, decisions are made on the validity of data and where sentences, paragraphs and pages belong.

Even though a block of text repeats many times each in its context, there is much textual analysis to determine true or false, genre of data, paragraphs that mean the same thing, same information derivative wording.

I have included the entire C code to generate such a library, textual analysis is lacking and the various wiki word and sentence dictionaries are not included.

Don't worry about the sheer amount of combinations, 90% of the generated content is gibberish and further textual analysis would make it even smaller. Building dictionaries of words and sentences might be useful for an...

A.I. yes, I said it. Artificial intelligence that curates the library of babel and acts as a librarian.

There are several different types of A.I. that could be usable.

Most people have witnessed generative A.I. Generative A.I. could perform a vast range of activities on the data. Omission of junk data and finding definitions of new words and making sense of text. The data itself could further be of a grade that it could be used as training data. It could consider future text and text beyond human understanding to be valid and explain weird concepts prompted by some data in the library.

The library would hold an infinite range of permutations on a prospective question to be true. Is the data true or false and not in politics but science. Is a mathematical formula true, is a chemistry molecule for some indication effective. An A.I. would have to determine true and false, a truth engine.

A.I. to perform as a search engine possibly built using genetic algorithms, machine learning.

Some A.I. techniques...

* Semantic reasoning AI
* Probabilistic inference like Bayesian inference
* Generating knowledge Graphs, Fact-Checking, visualization tools could uncover unknown patterns in data.
* Natural language processing NLP
* Custom A.I. specially for the task.
* Entertaining ways of representing data. 

The role of A.I. is kind of incredible, it has to choose to return results that are most likely to be true in our timeline, true in our physics. By training an A.I. to return validate facts from history, math and science, truth statements, mathematical truths, boolean truths would it more be capable of returning future or unvalidable potential facts. If a genetic algorithm was trained to be able to choose among many options from the past, could it be just as effective in grabbing the right page that describes the future.

Facebook has an A.I. named prophet that predicts future trends trained on past trends, forecasting time series data. Could a similar A.I. return the likelihood of future tense articles if trained on factual past tense articles.

So guys, there it is, my attempt at improving the library of babel. Source code links are in the comments.

Tony over and out
