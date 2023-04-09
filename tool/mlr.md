mlr
	1. mlr help topics to see if it worked
	2. 	mlr [input/output file formats] [verbs] [file]

		examples: 
			mlr --csv filter '$color != "red"' example.csv
			mlr --csv head ./tv_ratings.csv`
			mlr --csv --opprint head ./tv_ratings.csv
			mlr --c2p cut -x -f titleId then head ./tv_ratings.csv
			mlr --c2p sort -nr av_rating then head ./tv_ratings.csv
    3. --c2p ==> --csv --opprint
	4. Chaining 
		mlr --c2p cut -x -f titleId then head ./tv_ratings.csv

	5. csv to json
		mlr --c2j sort -nr av_rating ./tv_ratings.csv > sorted.json

	6. cal
		mlr --c2p put '$medals=$bronze+$silver+$gold' \
    		then sort -nr medals \
    		then head -n 5 ./athletes.csv

