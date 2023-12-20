// $('.counter').counterUp({
//     delay: 10,
//     time: 300
// });

const dbRefObject = firebase.database().ref();
dbRefObject.on('value', function (snapshot) {
    let counter_value = snapshot.child("muhammadi_counter").val(); // 
    let time_value = snapshot.child("muhammadi_time").val(); // 

    // console.log(counter_value);
    // console.log(time_value);
    
    document.getElementById('count_id').innerHTML =  counter_value;
    document.getElementById('date_id').innerHTML =  "Last Updated: " + time_value;

    });


