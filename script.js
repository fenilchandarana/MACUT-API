// Firebase Database URL
const firebaseUrl = "https://macut-api-default-rtdb.firebaseio.com/data.json";

// Function to fetch data from Firebase and display it in the respective display-box
function fetchData() {
    fetch(firebaseUrl)
        .then(response => response.json())
        .then(data => {
            // Access the data under the "data" object
            const file1Content = data && data.file1 ? data.file1 : "No data found for File 1";
            const file2Content = data && data.file2 ? data.file2 : "No data found for File 2";

            // Update the content of the display-boxes
            document.getElementById("file1-display").innerText = file1Content;
            document.getElementById("file2-display").innerText = file2Content;
        })
        .catch(error => console.error("Error fetching data:", error));
}

// Fetch data from Firebase when the page loads
window.onload = fetchData;

// Optional: You can also add this function to reload data every few seconds
setInterval(fetchData, 5000); // Fetch new data every 5 seconds
