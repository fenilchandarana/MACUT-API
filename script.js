document.getElementById('send-button').addEventListener('click', () => {
    const inputText = document.getElementById('input-box').value;
    document.getElementById('display-box').innerText = inputText;
    document.getElementById('input-box').value = ''; // Clear the input box
});

// Firebase Database URL
const firebaseUrl = "https://macut-api-default-rtdb.firebaseio.com/data.json";

// Function to send data to Firebase
function sendData() {
    const inputBox = document.getElementById("input-box");
    const message = inputBox.value;

    // Send the message to Firebase
    fetch(firebaseUrl, {
        method: "PUT",  // "PUT" will replace the entire data at the "data.json" path
        headers: {
            "Content-Type": "application/json",
        },
        body: JSON.stringify({ message: message })  // Send message as a JSON object
    })
    .then(response => {
        if (response.ok) {
            console.log("Message sent to Firebase:", message);
            inputBox.value = ""; // Clear the input box after sending
        }
    })
    .catch(error => console.error("Error sending data:", error));
}

// Event listener for the "Send" button
document.getElementById("send-button").addEventListener("click", sendData);

