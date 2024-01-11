---
title: Redux Study I
date: 2023-09-10 21:15:20 +0900
categories: [Web, React]
tags: [react, redux]		# tags MUST be small letters, like tag NOT TAG 
---
# Redux Study Notes

### Terms in Redux

- **Action**: An object that indicates what to do. It must have an `Action type` attribute.
- **Action Creator**: Literally a factory for creating Actions.
- **Dispatch**: Sends the created Action to the Reducer to create changes in the Store.
- **Store**: Holds the state (State) of the application. There is typically one Store per app.
- **Reducer**: When a user dispatches an Action to change the Store's state, the Reducer transforms the old state into a new one based on the Action. Importantly, Reducers must be pure functions and should not have dynamic processing or changes within them.

### 3 Rules of Redux

1. **Single Source of Truth**: There should only be one Store in an app.
2. **States are Read-only**: State values must not be changed directly. They should be changed through Actions and Dispatch.
3. **Changes Made with Pure Functions**: Reducers must always be pure functions.

### Example: Basic React State Usage Code

```jsx
import { useState } from 'react';
import './App.css'; 

function Counter() {   // State: a counter value
  const [counter, setCounter] = useState(0) 

  const increment = () => {  // Action: code that causes an update to the state when something happens
    setCounter(prevCounter => prevCounter + 1)
  }

  return (   // View: the UI definition
    <div>
      Value: {counter} <button onClick={increment}>Increment</button>
    </div>
  )
}
export default Counter;
```

### Understanding State, View, and Actions in React

- **state**: The state of the app.
- **view**: The UI currently displayed.
- **actions**: Event triggers like `onClick`, leading to data flow in a one-way direction. State changes are singular in origin. Props inheritance is possible up to one or two levels.

However, the complexity increases when there are multiple state-changing functions located in different places.

### The Difference in State Handling Between C-like Languages and JavaScript React

In C-like languages, variables declared at a higher level work fine in lower functions, and global variables are easily used. However, in JavaScript React State, if you want to use a concept similar to global variables, you need to use Redux.

- **Redux**: A space in an app where all globally usable states are collected.

### Immutability in JavaScript

**Immutability** refers to the concept where an object or array does not change its state after it is created.

#### Mutating Object or Array

Even when using `const` to declare an object or array, it's important to note that the memory address is constant, but the contents can still be changed.

Example of Mutating an Object:
```jsx
const obj = { a: 1, b: 2 };
// The same object reference is maintained, but the contents have changed
obj.b = 3;
const arr = ['a', 'b'];
// Similarly, the contents of this array can be changed
arr.push('c');
arr[1] = 'd';
```
This demonstrates that while the variable (obj, arr) cannot be reassigned due to the const declaration, the contents of the object or array can still be mutated.

Declaring an object or array with `const` means only the memory address is constant; the contents can still be changed.

To set values immutably, use the **spread operator** (`...`) and array modification functions to create and modify copies of existing objects or arrays. This approach is akin to Blockchain.

```jsx
const obj = {
  a: {
    c: 3     // To safely update obj.a.c, we have to copy each piece
  },
  b: 2
}

const obj2 = {
  ...obj,   // copy obj
  a: {   // overwrite a
    ...obj.a,     // copy obj.a
    c: 42     // overwrite c
  }
}

const arr = ['a', 'b'] // Create a new copy of arr, with "c" appended to the end
const arr2 = arr.concat('c') // or, we can make a copy of the original array:
const arr3 = arr.slice() // and mutate the copy:
arr3.push('c')
```

**Action** is a JavaScript object with a **type** field, representing an event that occurred in the application. The `type` field should be a descriptive string, e.g., `todos/todoAdded` following the format domain/eventName, indicating the feature or category the action belongs to and the specific thing that happened.

The **Payload** contains additional information about what occurred, thus actions and action creators typically include a payload.

```jsx
const addTodoAction = {
    type: 'todos/todoAdded',
    payload: 'Buy milk'
  }
    
  const addTodo = text => {
    return {
      type: 'todos/todoAdded',
      payload: text
    }
  }
```

A **Reducer** receives the current state and an action object, decides how to update the state, and returns the new state. It is essentially an event listener that handles events based on the received action type.

Reducer Rules:

- They should only use state and action to produce a new state.
- Modifying the **existing** state is not allowed.
- Updates must be **immutable**, achieved by **copying** the existing state.
- Asynchronous logic is not permitted; synchronization is mandatory.
- Reducers must not produce random values or other side effects.

In Redux, the management of a global array or object involves programming in a way that prevents direct access to the original values for safety, then appending to update the state.

The current Redux application state resides in an object called **store**, which is created by the Reducer and can be accessed using a `getState` function.

Reducer Examples:
```jsx
const initialState = { value: 0 }

  function counterReducer(state = initialState, action) {
    if (action.type === 'counter/increment') { // If action.type matters 
      return {
        ...state,
        value: state.value + 1 // update to new value 
      }
    }
    return state // no change if not 
  }
```

In Redux, the management of global arrays or objects is designed for safety by programming in a way that prevents direct access to original values. Instead, updates are made by appending to the state.

The current Redux application state is contained within an object named **store**.

**Store** is created by the Reducer and its value can be returned using the `getState` function.
```jsx
import { configureStore } from '@reduxjs/toolkit'
const store = configureStore({ reducer: counterReducer })
console.log(store.getState())
```
**Dispatch** implies sending an action within the store. If the action is not filtered out by the Reducer, it updates the state value. Dispatch is essentially triggering an event, indicating that something has happened and the store value needs to be updated.

**Selector** is used to extract specific values from the state within the store.

To work with Redux effectively, installing the Redux DevTools Chrome Extension, a developer tool extension, is recommended.

**ConfigureStore** is used to create a Redux Store. When using Reducers, ConfigureStore is necessary.

A **Slice** combines reducers and actions into a single unit, often declared together in one file.
```jsx
import { configureStore } from '@reduxjs/toolkit'
import usersReducer from '../features/users/usersSlice'
import postsReducer from '../features/posts/postsSlice'
import commentsReducer from '../features/comments/commentsSlice'

export default configureStore({
  reducer: {
    users: usersReducer,
    posts: postsReducer,
    comments: commentsReducer
  }
})
```
Example of a Slice Reducer function: state.user, state.post, state.comments represent each slice of the state. As the state and its corresponding reducer are together, it's referred to as a slice reducer function.

Slice Reducer & Actions Generation Example: 
```jsx
import { createSlice } from '@reduxjs/toolkit'

export const counterSlice = createSlice({
  name: 'counter',
  initialState: {
    value: 0 // 이게 State 
  },
  reducers: { // State 를 변경할 수 있는 Reducer 들 
    increment: state => {
      state.value += 1
    },
    decrement: state => {
      state.value -= 1
    },
    incrementByAmount: (state, action) => {
      state.value += action.payload 
    } // Action 이 Reducer를 통과하는지 Payload로 확인 
  }   // Acton은 Type이랑 Payload 로 구성
})

export const { increment, decrement, incrementByAmount } = counterSlice.actions

export default counterSlice.reducer
```

Creating Slice Reducers & Actions involves defining a state name, setting an initial value, and specifying reducers that can update the state value. All these are contained in one object, which constitutes the slice. The situations that activate the reducer are the actions.

**Asynchronous** means not existing or happening at the same time.

Thunk is a Redux function capable of handling asynchronous logic. It contains Dispatch and getState.

The Creator Function must be external, and the Thunk Function can be created and returned.

```jsx
export const incrementAsync = amount => dispatch => {
  setTimeout(() => {
    dispatch(incrementByAmount(amount))
  }, 1000)
} // get state value by dispatch 
store.dispatch(incrementAsync(5))
```

Redux-thunk middleware must be present in the Redux store, but configureStore is already set up for this.

```jsx 
const fetchUserById = userId => { // Thuck generator 
  return async (dispatch, getState) => {
    try {
      const user = await userAPI.fetchById(userId) // dispatch if responses 
      dispatch(userLoaded(user))
    } catch (err) { // Error Handler 
    }
  }
}
```

Selectors return the current value of the state, for example, state.abcd.value.
