extends Control

onready var ctx = preload("res://interface/wcontext.gdns").new()

func _ready():
	$VersionLabel.text = ctx.version
	get_node("MainMenuPanel/VBoxContainer/ExitButton").connect("pressed", self, "_on_ExitButton_pressed")


func _on_ExitButton_pressed():
	get_tree().quit()