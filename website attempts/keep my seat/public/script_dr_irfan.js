const dbRefObject = firebase.database().ref();
dbRefObject.on('value', function (snapshot) {
    let counter_value = snapshot.child("dr_irfan_counter").val(); // 
    let time_value = snapshot.child("dr_irfan_time").val(); // 

    // console.log(counter_value);
    // console.log(time_value);
    
    document.getElementById('count_id').innerHTML =  counter_value;
    document.getElementById('date_id').innerHTML =  "Last Update: " + time_value;

    });


