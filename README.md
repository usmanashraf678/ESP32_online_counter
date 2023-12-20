# KeepMySeat Brief
`KeepMySeat` was envisioned to reduce the patient waiting time for local clinics in Pakistan. This github repo houses the ESP32 codebase (for both Controller and Large SSD Display), website, PCB designs and other aspects of the project.

# How it works?
KeepMySeat provided an online channel (via keepmyseat.com website) to check the current patient number being served a certain clinic. The website code is present inside the `website/` folder and is built on top of bootstrap. The home page looks as follows:
![image](https://github.com/usmanashraf678/counter_online/assets/29997321/de2ce210-0a2d-452c-8458-291b2cecaafc)

Each individual page for a doctor looked as follows:
![image](https://github.com/usmanashraf678/counter_online/assets/29997321/62349f30-65aa-411d-839d-6ca83cfae2a1)

Using this page, a patient could see in real-time what number is being served in a given clinic eliminating their need to wait in person on the clinic.

# How the data was generated?
The patient counter was maintained at the doctor's clinic either by the doctor themselves or their attendant. The controller was connected to wifi internet using the ESP32 board mounted. Once the counter was changed, this information was relayed to a large (3 digit) SSD display in the doctor's clinic for patients' information. This large SSD was also equipped with a ESP32 to receive information over the internet. The counter update was also transmitted in real-time to the website database. This would allow the patients to keep track of their turn while not being physically present in the clinic.


# Response
This project did not go particularly well on the field because of resistance from the stakeholders involved to embrace this change. The patients were pretty happy with this facility. But, there appeared to be a possible conflict of interest from some parties involved such as attendants. Sometimes, the doctors were technology averse and did not want to "complicate" their operations. Other times, they lacked empathy to think about the troubles patients face while waiting for their turn.

# Next Steps
I am archiving this project for now. However, if you want to take advantage of the work done till now. You are welcome!
