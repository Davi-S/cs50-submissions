SELECT p.name FROM people p
JOIN stars s ON p.id = s.person_id
JOIN movies m ON m.id = s.movie_id
WHERE m.title = "Toy Story"