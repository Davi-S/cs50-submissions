SELECT p.name
FROM people p
JOIN stars s ON s.person_id = p.id
JOIN movies m ON m.id = s.movie_id
WHERE m.title IN
(
SELECT m.title
FROM movies m
JOIN stars s ON s.movie_id = m.id
JOIN people p ON p.id = s.person_id
WHERE p.name = "Kevin Bacon" AND p.birth = 1958
)
AND p.name != "Kevin Bacon"