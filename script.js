document.getElementById('send-button').addEventListener('click', () => {
    const inputText = document.getElementById('input-box').value;
    document.getElementById('display-box').innerText = inputText;
    document.getElementById('input-box').value = ''; // Clear the input box
});

// Firebase Database URL
const firebaseUrl = "https://macut-api-default-rtdb.firebaseio.com/data.json";

// Function to fetch data from Firebase and display it in `display-box`
function fetchData() {
    fetch(firebaseUrl)
        .then(response => response.json())
        .then(data => {
            // Display only the message content
            const displayText = data.message || "No message found"; // Access the "message" field directly
            document.getElementById("display-box").innerText = displayText;
        })
        .catch(error => console.error("Error fetching data:", error));
}

// Fetch data from Firebase when the page loads
window.onload = fetchData;

// Optional: You can also add this function to reload data every few seconds
setInterval(fetchData, 5000); // Fetch new data every 5 seconds