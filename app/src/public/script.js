document.getElementById("inputForm").addEventListener("submit", async function(event) {
    event.preventDefault();

    let inputField = document.getElementById("userInput");
    let inputText = inputField.value.trim().toLowerCase();

    let response = await fetch("/process", {
        method: "POST",
        headers: { "Content-Type": "application/x-www-form-urlencoded" },
        body: "user_input=" + encodeURIComponent(inputText)
    });

    let result = await response.text();
    document.getElementById("output").innerHTML = result.replace(/\n/g, "<br>");

    inputField.value = "";
});
